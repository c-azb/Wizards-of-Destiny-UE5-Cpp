

#include "Characters/BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Wizard/WizardPlayerState.h"

#include "NiagaraFunctionLibrary.h"	


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::OnRep_Controller() //client call
{
	Super::OnRep_Controller();
	InitializeCharacter();
}

void ABaseCharacter::PossessedBy(AController* NewController) //server call
{
	Super::PossessedBy(NewController);
	InitializeCharacter();
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	PlayBloodVFX(DamageEvent,EventInstigator->GetPawn());
	return Damage;
}

bool ABaseCharacter::IsAlive()
{
	return false;// ResourcesComponent->IsAlive();
}

void ABaseCharacter::InitializeCharacter()
{
}

void ABaseCharacter::PlayMontage(TObjectPtr<UAnimMontage> AnimMontage, const int32 SectionCount, const bool bPlayRandom,const float StartTime)
{
	if (AnimMontage)
	{
		FName SecName;
		if (bPlayRandom)
			SecName = FName(FString::Printf(TEXT("Sec%d"), FMath::RandRange(1, SectionCount)));
		else
			SecName = FName(FString::Printf(TEXT("Sec%d"), SectionCount));

		UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimMontage && AnimInstance)
		{
			float const Duration = AnimInstance->Montage_Play(AnimMontage, 1,EMontagePlayReturnType::MontageLength,StartTime);

			if (Duration > 0.f && SecName != NAME_None && SectionCount > 0)
			{
				AnimInstance->Montage_JumpToSection(SecName, AnimMontage);
			}
		}

	//	PlayAnimMontage(AnimMontage, 1, FName(SecName));
	}
}

void ABaseCharacter::PlayVoiceLineAttackMontage(const bool bIncludeSpellVL)
{
	if (FMath::RandRange(.0f, 1.f) < 0.5f)
	{
		float RandValue = FMath::RandRange(.0f, 1.f);
		if (VL_AttackSpellMontage && bIncludeSpellVL && RandValue < .2f)
		{
			PlayMontage(VL_AttackSpellMontage, VL_AttackSepellSecsCount, true);
		}
		else if(VL_AttackNeutralMontage && RandValue < .75f)
		{
			PlayMontage(VL_AttackNeutralMontage, VL_AttackNeutralSecsCount, true);
		}
		else
		{
			PlayVoinceLineGrllsAction();
		}
	}
}

void ABaseCharacter::PlayVoinceLineGrllsAction(const float ProbChange)
{
	if (VL_GrllsActionMontage && FMath::RandRange(.0f, 1.f) < ProbChange)
	{
		PlayMontage(VL_GrllsActionMontage, 2, true);
	}
}

void ABaseCharacter::PlayVoiceLinePainMontage()
{
	if (VL_PainMontage && FMath::RandRange(.0f, 1.f) < 0.5f)
	{
		float RandValue = FMath::RandRange(.0f, 1.f);
		if (RandValue < .55f)
		{
			PlayMontage(VL_PainMontage, VL_PainSecsCount, true);
		}
		else
		{
			PlayVoinceLineGrllsPain();
		}
	}
}

void ABaseCharacter::PlayVoinceLineGrllsPain(const float ProbChange)
{
	if (VL_GrllsPainMontage && FMath::RandRange(.0f, 1.f) < ProbChange)
	{
		PlayMontage(VL_GrllsPainMontage, 2, true);
	}
}

void ABaseCharacter::EnableRagDoll()//(const FVector& HitPosition, const float DamageApplyed)
{
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	//const float ForceAmount = DamageApplyed * 100;
	//FVector ForceDir = (GetActorLocation() - HitPosition).GetSafeNormal();
	//GetMesh()->AddImpulseAtLocation(ForceDir * ForceAmount, HitPosition);
}

void ABaseCharacter::BaseOnGetDamage(const AActor* DamageCauser, const float DamageAmount)
{
	if (IsAlive() && DamageAmount >= 30)
	{
		float DirsDotProduct = GetActorForwardVector().Dot(DamageCauser->GetActorRotation().Vector());
		OnGetHitHard(DirsDotProduct);
	}
	else if(!IsAlive())
	{
		EnableRagDoll();
	}
}

void ABaseCharacter::SendKillEvent(const AController* InstigatorCtrl,const EWizardScore KillType)
{
	if (!IsAlive())
	{
		if (AWizardPlayerState* MyPlayerState = Cast<AWizardPlayerState>(GetPlayerState()))
		{
			MyPlayerState->AddScore(EWizardScore::EWS_Dead, 1);
		}

		if (InstigatorCtrl)
		{
			if (AWizardPlayerState* OtherPlayerState = InstigatorCtrl->GetPlayerState<AWizardPlayerState>())
			{
				OtherPlayerState->AddScore(KillType, 1);
			}
		}
	}
}


void ABaseCharacter::OnGetHitHard(const float DotProductDir)
{
	if (DotProductDir > 0)
	{
		PlayMontage(GetHitHardMontage,1,false);
	}
	else
	{
		PlayMontage(GetHitHardMontage, 2, false);
	}
}

void ABaseCharacter::OnRepResources(){}

void ABaseCharacter::OnGetPickUp(const EPickUpType PickUpType,const uint32 PickUpId)
{
	if (AWizardPlayerState* MyPlayerState = Cast<AWizardPlayerState>(GetPlayerState()))
	{
		MyPlayerState->AddScore(EWizardScore::EWS_Pickup, 1);
		MyPlayerState->AddPickUp(PickUpId, PickUpType);
	}
}

void ABaseCharacter::PlayBloodVFX(FDamageEvent const& DamageEvent,const AActor* Instigator_)
{
	const bool bCanSpawnVFX = (GetWorld()->GetRealTimeSeconds() - SpawnBloodVFXTime) > .5f || SpawnBloodVFXTime == 0;
	if (BloodVFX && Instigator_ && bCanSpawnVFX)
	{
		SpawnBloodVFXTime = GetWorld()->GetRealTimeSeconds();

		FHitResult HitResult;
		FVector ImpulseDir;
		DamageEvent.GetBestHitInfo(this, Instigator_, HitResult, ImpulseDir);
		UNiagaraFunctionLibrary::SpawnSystemAttached(BloodVFX, GetMesh(), FName(), HitResult.ImpactPoint, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
	}
}


