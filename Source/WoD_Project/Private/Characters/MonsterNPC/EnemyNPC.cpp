// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/EnemyNPC.h"
#include "Characters/Wizard/ResourcesComponent.h"
#include "Characters/MonsterNPC/AIEnemyController.h"
#include "GameTypes/ActorTagsDef.h"

#include "Perception/AISense_Damage.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Items/HoldItem.h"
#include "Items/HealthComponent.h"

#include "GameManagers/WoDGameInstance.h"
#include "GameTypes/BaseItemStruct.h"

#include "WizardWorldUtils/WorldEnemySpawner.h"

#include "Net/UnrealNetwork.h"

AEnemyNPC::AEnemyNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	HealthComponent->SetIsReplicated(true);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 350;
	bUseControllerRotationPitch = bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
	WalkSpeed = 350;
	InCombatSpeed = 200;

	Tags.Add(MONSTER_TAG);
}

void AEnemyNPC::InitializeCharacter()
{
	Super::InitializeCharacter();
	if (HasAuthority())
	{
		HealthComponent->InicializeResource();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = SpawnParameters.Instigator = this;
		EnemyWeaponInst = GetWorld()->SpawnActor<AHoldItem>(EnemyWeapon, FTransform(), SpawnParameters);
		EnemyWeaponInst->AttachHoldItem(GetMesh());
	}
}


void AEnemyNPC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyNPC, EnemyState);
}

float AEnemyNPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!HasAuthority() || !IsAlive()) return 0;

	HealthComponent->GetDamage(DamageTaken);

	if (IsAlive())
	{
		FHitResult HitResult;
		FVector ImpulseDir;
		DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDir);
		APawn* PawnInstigator = EventInstigator->GetPawn();
		UAISense_Damage::ReportDamageEvent(this, this, PawnInstigator, DamageTaken, DamageCauser->GetActorLocation(), HitResult.ImpactPoint);	
	}
	else if (GetAIController())
	{
		AIEnemyController->DisableAI();
		EndAttack();
		DropGrocery();
		SendKillEvent(EventInstigator, EWizardScore::EWS_Monster_Kill);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle,this, &ThisClass::EndDeathTimer, 5);
	}
	BaseOnGetDamage(DamageCauser, DamageTaken);

	return DamageTaken;
}

bool AEnemyNPC::IsAlive()
{
	return HealthComponent->IsAlive();
}

bool AEnemyNPC::SetEnemyState(const EEnemyState NewEnemyState)
{
	if (EnemyState == NewEnemyState)
	{
		return false;
	}
	EnemyState = NewEnemyState;
	SetMovementState();
	PlayMontageBasedOnState();
	return true;
}

void AEnemyNPC::SetMovementState()
{
	bUseControllerRotationYaw = (EnemyState >= EEnemyState::EES_CloseAttack && EnemyState != EEnemyState::EES_Death) ||
							(GetVelocity().SizeSquared2D() <= 25 && EnemyState == EEnemyState::EES_Chasing);
	GetCharacterMovement()->bOrientRotationToMovement = !bUseControllerRotationYaw;
	GetCharacterMovement()->MaxWalkSpeed = GetMaximunSpeed();
}

void AEnemyNPC::SetupAttack(const bool bJumpAtk)
{
	GetCharacterMovement()->bCheatFlying = bJumpAtk;

	if (HasAuthority() && bJumpAtk)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

void AEnemyNPC::Attack(const float DamageMult, const float DamageRangeMult)
{
	if (HasAuthority() && EnemyWeaponInst)
	{
		EnemyWeaponInst->ActivateItemCustom(DamageMult,DamageRangeMult);
	}
}

void AEnemyNPC::EndAttack()
{
	if (!HasAuthority()) return;

	if (EnemyWeaponInst)
	{
		EnemyWeaponInst->DeactivateItem();
	}

	if (IsAttacking() && GetAIController())
	{
		AIEnemyController->SetEnemyState(EEnemyState::EES_Chasing);
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}


bool AEnemyNPC::GetActorsInAtkRange(TArray<FOverlapResult>& OutActors,const EEnemyState AttackType)
{
	FVector3f BoxSize;
	float BoxPosOffset;

	BoxSize = AttackType == EEnemyState::EES_CloseAttack ? CloseAtkBoxSize : FarAtkBoxSize;
	BoxPosOffset = AttackType == EEnemyState::EES_CloseAttack ?  0 : FarAttackRange;

	if (BoxSize.X <= 0) return false;
	else if (AttackType == EEnemyState::EES_FarAttack && FMath::RandRange(.0f, 1.0f) >= FarAttackProbability) return false;

	float NPCRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float NPCHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	FVector BoxPos = GetActorLocation() + GetActorForwardVector() * (BoxSize.X + NPCRadius + BoxPosOffset) + FVector::UpVector * (NPCHalfHeight - BoxSize.Z);
	FQuat BoxRot = FQuat(GetActorRotation());
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(BoxSize);
	FCollisionQueryParams CollisionQuerry;
	CollisionQuerry.AddIgnoredActor(this);

	DrawDebugBox(GetWorld(), BoxPos, FVector(BoxSize), BoxRot, FColor::Red);
	return GetWorld()->OverlapMultiByChannel(OutActors, BoxPos, BoxRot, ECollisionChannel::ECC_WorldDynamic, CollisionShape, CollisionQuerry);
}

float AEnemyNPC::GetMaximunSpeed()
{
	if (EnemyState == EEnemyState::EES_Chasing)
	{
		return WalkSpeed * RunSpeedMult;
	}
	else if (EnemyState == EEnemyState::EES_Patrolling)
	{
		return WalkSpeed;
	}
	else if (EnemyState == EEnemyState::EES_Death)
	{
		return 0;
	}

	return InCombatSpeed;
}

void AEnemyNPC::PlayMontageBasedOnState()
{
	if (EnemyState == EEnemyState::EES_CloseAttack)
	{
		PlayMontage(CloseAttackMontage, CloseAttacksCount, CloseAttacksCount > 1);
	}
	else if (EnemyState == EEnemyState::EES_FarAttack)
	{
		PlayMontage(FarAttackMontage, FarAttacksCount, FarAttacksCount > 1);
	}
	else if (EnemyState == EEnemyState::EES_GetHitFront)
	{
		PlayMontage(GetHitHardMontage, 1, false);
	}
	else if (EnemyState == EEnemyState::EES_GetHitBack)
	{
		PlayMontage(GetHitHardMontage, 2, false);
	}
}

void AEnemyNPC::OnGetHitHard(float DotProductDir)
{
	if (GetAIController())
	{
		EndAttack();
		if (DotProductDir < 0)	//if came from front, character and damager has oposite directions
		{
			AIEnemyController->SetEnemyState(EEnemyState::EES_GetHitFront);
		}
		else
		{
			AIEnemyController->SetEnemyState(EEnemyState::EES_GetHitBack);
		}
	}
}

void AEnemyNPC::EndGetHitHard()
{
	if (IsGettingHitHard() && GetAIController())
	{
		AIEnemyController->SetEnemyState(EEnemyState::EES_Chasing);
	}
}

bool AEnemyNPC::GetAIController()
{
	if (!AIEnemyController)
	{
		AIEnemyController = Cast<AAIEnemyController>(GetController());
	}
	
	return AIEnemyController != nullptr;
}

bool AEnemyNPC::IsGettingHitHard()
{
	return EnemyState >= EEnemyState::EES_GetHitFront && EnemyState <= EEnemyState::EES_GetHitBack;
}

bool AEnemyNPC::IsAttacking()
{
	return (EnemyState >= EEnemyState::EES_CloseAttack && EnemyState <= EEnemyState::EES_FarAttack);
}

void AEnemyNPC::DropGrocery()
{
	if (!HasAuthority() || FMath::RandRange(0.0f, 1.0f) >= .65f)
	{
		return;
	}

	EGroceryValue GroceryValue = FMath::RandRange(0.0f, 1.0f) <= .25f ? EGroceryValue::EGV_Full : EGroceryValue::EGV_Half;

	if (UWoDGameInstance* WoDGameInstance = GetGameInstance<UWoDGameInstance>())
	{
		uint32 GroceryCode = 0;
		FBaseItem* BaseItem = nullptr;

		do
		{
			BaseItem = WoDGameInstance->SearchItem(GroceryCode, EHoldItem::EHI_Grocery);
			if (BaseItem)
			{
				if (FGroceryData* GroceryData = static_cast<FGroceryData*>(BaseItem))
				{
					if (DropGrocerysOptions.Contains(GroceryData->GroceryType) && GroceryData->GroceryValue == GroceryValue && FMath::RandRange(0.0f,1.0f) < .45f)
					{
						GetWorld()->SpawnActor<AActor>(GroceryData->ItemClass, GetActorLocation(), FRotator::ZeroRotator);
						return;
					}
				}
			}
			
			GroceryCode++;

		} while (BaseItem != nullptr);

	}
}

void AEnemyNPC::EndDeathTimer()
{
	if (AWorldEnemySpawner* WorldEnemySpawner = Cast< AWorldEnemySpawner>(GetOwner()))
	{
		WorldEnemySpawner->RemoveEnemyFromWorld(this);
	}
	else
	{
		Destroy();
	}
}

void AEnemyNPC::OnRep_EnemyState()
{
	PlayMontageBasedOnState();
}



