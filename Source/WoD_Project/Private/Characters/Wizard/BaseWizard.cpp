// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/BaseWizard.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/Wizard/WizardItemsComponent.h"
#include "Characters/Wizard/ResourcesComponent.h"

#include "Net/UnrealNetwork.h"
#include "GameTypes/ActorTagsDef.h"
#include <GameTypes/ResourcesStruct.h>

#include "Characters/Wizard/WizardPlayerState.h"


ABaseWizard::ABaseWizard()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>(FName("ResourcesComponent"));
	ResourcesComponent->SetIsReplicated(true);

	ItemsComponent = CreateDefaultSubobject<UWizardItemsComponent>(FName("ItemsComponent"));
	ItemsComponent->SetIsReplicated(true);

	Tags.Add(WIZARD_TAG);
}

void ABaseWizard::InitializeCharacter()
{
	Super::InitializeCharacter();
	if (HasAuthority())
	{
		ResourcesComponent->InicializeResource();
	}
	SetupMovementType();
}

void ABaseWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessContinuousResources(DeltaTime);
	SetFlyingSpeedBasedOnGround(DeltaTime);
}

void ABaseWizard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseWizard, WizardState);
	DOREPLIFETIME(ABaseWizard, IsRunning);
	DOREPLIFETIME(ABaseWizard, IsDashing);
}

void ABaseWizard::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	//duplicated function from the engine
	// adapted to rotate smoothly on 1st person movement

	if (bUseControllerRotationYaw)
	{
		const FRotator CurrentRotation = GetActorRotation();
		NewControlRotation.Roll = CurrentRotation.Roll;

		if (!bUseControllerRotationPitch)
		{
			NewControlRotation.Pitch = 0;
		}

		float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees<float, float>(NewControlRotation.Yaw, CurrentRotation.Yaw));
		float Speed = (1 - YawDiff / 360) * 20;
		NewControlRotation = FMath::RInterpTo(CurrentRotation, NewControlRotation, DeltaTime, Speed);

		SetActorRotation(NewControlRotation);
	}
}

bool ABaseWizard::GetWizardPlayerState()
{
	if (WizardPlayerState == nullptr)
	{
		WizardPlayerState = GetPlayerState<AWizardPlayerState>();
	}
	return WizardPlayerState!=nullptr;
}

int32 ABaseWizard::GetDefaultEquippedItem(const EHoldItem ItemType)
{
	if (GetWizardPlayerState())
	{
		return WizardPlayerState->GetEquippedItem(ItemType);
	}
	return 0;
}

void ABaseWizard::SetDefaultEquippedItem(const EHoldItem ItemType, const int32 ItemId)
{
	if (GetWizardPlayerState())
	{
		WizardPlayerState->SetEquippedItem(ItemType,ItemId);
	}
}

void ABaseWizard::ProcessContinuousResources(const float DeltaTime)
{
	if (HasAuthority())
	{
		if (IsActivatingItem())
		{
			FResourcesStruct ContinuousItemCost;
			if (ItemsComponent->GetContinuousItemCost(ContinuousItemCost))
			{
				float Mult = ItemsComponent->GetCurrentItem() == EHoldItem::EHI_FlyingBroom ? FMath::Clamp((GetVelocity().Length() / GetMaxSpeed()), 0, 1) : 1;

				if (!ResourcesComponent->ConsumeContinuousResources(ContinuousItemCost, Mult, DeltaTime))
				{
					DeactivateItem(true);
				}
			}
		}
		ItemsComponent->FlyingBroomDelayCounter(DeltaTime);
		ResourcesComponent->AutoReloadMana(DeltaTime);

		if (IsRunning)
		{
			FResourcesStruct StaminaConsume(0, RunStaminaCost * DeltaTime,0);
			IsRunning = ResourcesComponent->ConsumeResources(StaminaConsume, 1);
			SetWizardSpeed();
		}
		else if (!IsDashing)
		{
			ResourcesComponent->AutoReloadStamina(DeltaTime);
		}
	}
}

void ABaseWizard::SetFlyingSpeedBasedOnGround(const float DeltaTime)
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
	{
		FHitResult HitResult;
		FVector EndLoc = GetActorLocation() + FVector::DownVector * 100;
		FCollisionQueryParams CollQuery;
		CollQuery.AddIgnoredActor(this);
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EndLoc, ECollisionChannel::ECC_Visibility, CollQuery);
		float MaxFlySpeedTarget = bHit ? MaxFlySpeed / (FMath::Clamp(100 - HitResult.Distance, 1, 10)) : MaxFlySpeed;
		GetCharacterMovement()->MaxFlySpeed = FMath::Lerp<float, float>(GetCharacterMovement()->MaxFlySpeed, MaxFlySpeedTarget, DeltaTime * 35);
	}
}

float ABaseWizard::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!HasAuthority() || !ResourcesComponent->IsAlive()) return 0;
	
	ResourcesComponent->GetDamage(Damage);

	if (!IsAlive())
	{
		SendKillEvent(EventInstigator, EWizardScore::EWS_Wizard_Kill);
		OnWizardDead();
	}
	else
	{
		BaseOnGetDamage(DamageCauser, Damage);
	}

	return Damage;
}

bool ABaseWizard::IsAlive()
{
	return ResourcesComponent->IsAlive();
}

void ABaseWizard::InitializeItems(const uint32 MWCode)
{
	if (GetWizardPlayerState())
	{
		WizardPlayerState->SetEquippedItem(EHoldItem::EHI_MagicWand, MWCode);
		ItemsComponent->SetupChangeItem(MWCode, EHoldItem::EHI_MagicWand, GetWizardLevel());
		ItemsComponent->ChangeItem(GetMesh());
	}
}

void ABaseWizard::AddSpellCombo(const ESpellComboDir ComboDir)
{
	if (!CanMakeAction(EWizardState::EWCS_DoingCombo,true)) return;
	if (ItemsComponent->AddSpellCombo((uint8)ComboDir))
	{
		SetWizardState(EWizardState::EWCS_DoingCombo);
		PlayComboMontage(ComboDir);
	}
}

void ABaseWizard::PlayComboMontage(const ESpellComboDir ComboDir)
{
	uint32 AnimId = ComboDir == ESpellComboDir::ESCD_Up ? 1 : ComboDir == ESpellComboDir::ESCD_Down ? 2 : ComboDir == ESpellComboDir::ESCD_Right ? 3 : 4;
	PlayMontage(ComboDirMontage, AnimId, false);
}

EActivateItem ABaseWizard::SetupItemActivation()
{
	EActivateItem ActivationType = EActivateItem::EAI_None;

	if (CanMakeAction(EWizardState::EWCS_ActivatingItemMIN) && !IsActivatingItem())
	{
		ActivationType = ItemsComponent->SetupItemActivation(GetWizardLevel());
		if (!ResourcesComponent->CanConsumeResources(ItemsComponent->GetItemCost(false)))
		{
			ActivationType = EActivateItem::EAI_None;
		}
	}
	if (ActivationType == EActivateItem::EAI_None)
	{
		ItemsComponent->ClearSpellCombo();
		OnSpellComboTimeout(false);
		DeactivateItem(true);
	}
	return ActivationType;
}

void ABaseWizard::StartActivateItem()
{
	if (CanMakeAction(EWizardState::EWCS_ActivatingItemMIN) && !IsActivatingItem())
	{
		EActivateItem ActivationType = ItemsComponent->GetItemActivateType();
		if (ActivationType != EActivateItem::EAI_None)
		{
			EWizardState NewState = (EWizardState)((uint8)ActivationType + (uint8)EWizardState::EWCS_ActivatingItemMIN);
			SetWizardState(NewState);
			
			PlayMontageBasedOnState();
			if (ActivationType == EActivateItem::EAI_HoldSpell)
			{
				ActivateItem();
			}
		}
	}
	ItemsComponent->ClearSpellCombo();
}

void ABaseWizard::ActivateItem()
{
	if (HasAuthority())
	{
		if (ResourcesComponent->ConsumeResources(ItemsComponent->GetItemCost(false),1))
		{
			ItemsComponent->ActivateItem();
		}
	}
}

void ABaseWizard::DeactivateItem(const bool bCancel)
{
	if (HasAuthority())
	{
		if (IsActivatingItem())
		{
			SetWizardState(EWizardState::EWCS_Combat);
		}
		if (ItemsComponent->GetItemActivateType() == EActivateItem::EAI_HoldSpell)
		{
			StopAnimMontage(HoldSpellMontage);
		}
		if (ItemsComponent->GetItemActivateType() == EActivateItem::EAI_DrinkPotion && !bCancel)
		{
			FBoostParameters BoostParameters = ItemsComponent->GetItemBoost();
			//process drink potion boost
			//other items boosts is added as permanent on character
		}
		if (ItemsComponent->GetCurrentItem() == EHoldItem::EHI_FlyingBroom)
		{
			StartChangeItem(GetDefaultEquippedItem(EHoldItem::EHI_MagicWand), EHoldItem::EHI_MagicWand);
		}
		
		ItemsComponent->DeactivateItem(bCancel,GetDefaultEquippedItem(EHoldItem::EHI_MagicWand));
	}
}

bool ABaseWizard::IsChagingItem()
{
	return WizardState > EWizardState::EWCS_ChangeItem_MIN && WizardState < EWizardState::EWCS_ChangeItem_MAX;
}

void ABaseWizard::StartChangeItem(const uint32 ItemId, const EHoldItem ItemType)
{
	if (CanMakeAction(EWizardState::EWCS_ChangeItem_MIN) && GetWizardPlayerState())
	{
		if (ItemsComponent->SetupChangeItem(ItemId, ItemType, GetWizardLevel()))
		{
			if (ResourcesComponent->CanConsumeResources(ItemsComponent->GetItemCost(true)))
			{
				EWizardState NewState = ItemType == EHoldItem::EHI_FlyingBroom ? EWizardState::EWCS_ChangeItem_FlyingBroom : EWizardState::EWCS_ChangeItem_Base;
				SetWizardState(NewState);
			
				ItemsComponent->ClearSpellCombo();
				PlayMontageBasedOnState();
				WizardPlayerState->SetEquippedItem(ItemType, ItemId);
				DeactivateItem(true);
			}
		}
	}
}


void ABaseWizard::ChangeItem()
{
	if (HasAuthority())
	{
		ItemsComponent->ChangeItem(GetMesh());
	}
}

void ABaseWizard::EndChangeItem()
{
	if (HasAuthority())
	{
		if (WizardState == EWizardState::EWCS_ChangeItem_Base)
		{
			SetWizardState(EWizardState::EWCS_Combat);
		}
		else if (WizardState == EWizardState::EWCS_ChangeItem_FlyingBroom)
		{
			SetWizardState(EWizardState::EWCS_FlyingBroom);
		}
	}
}

void ABaseWizard::OnSpellComboTimeout(const bool bActivateItemOnTimeOut)
{
	if (WizardState == EWizardState::EWCS_DoingCombo)
	{
		if (bActivateItemOnTimeOut)
		{
			StartActivateItem();
		}
		else
		{
			SetWizardState(EWizardState::EWCS_Combat);
		}
	}
}

bool ABaseWizard::CanMakeAction(const EWizardState NewWizardState, const bool bIncludeEqual)
{
	return (NewWizardState > WizardState || (bIncludeEqual && NewWizardState == WizardState)) && !IsDashing;
}

bool ABaseWizard::IsActivatingItem()
{
	return WizardState > EWizardState::EWCS_ActivatingItemMIN && WizardState < EWizardState::EWCS_ActivatingItemMAX;
}

bool ABaseWizard::CanMove()
{
	return WizardState != EWizardState::EWCS_ChangeItem_FlyingBroom && !IsDashing && 
		WizardState != EWizardState::EWCS_Dead && !IsGettingHitHard();
}

void ABaseWizard::OnWizardDead()
{
	if (WizardState != EWizardState::EWCS_Dead)
	{
		DeactivateItem(true);
		EnableRagDoll();
		if (HasAuthority())
		{
			SetWizardState(EWizardState::EWCS_Dead);
			PlayMontageBasedOnState();
		}
	}
}

void ABaseWizard::Dash()
{
	const bool bCanDash = 
		WizardState != EWizardState::EWCS_FlyingBroom && 
		WizardState != EWizardState::EWCS_Dead && WizardState != EWizardState::EWCS_DrinkingPotion &&
		WizardState != EWizardState::EWCS_ThrowingPotion && !IsGettingHitHard() && !GetCharacterMovement()->IsFalling();

	if (bCanDash && !IsDashing && ResourcesComponent->ConsumeResources(FResourcesStruct(0,DashStaminaCost,0),1))
	{
		IsDashing = true;
		SetupMovementType();
		PlayMontage(DashMontage, 0, false,IsRunning ? .2f : 0);

		if (IsChagingItem())
		{
			ChangeItem();
			EndChangeItem();
		}
	}
}

void ABaseWizard::Run(const bool bIsRunClick)
{
	if (!bIsRunClick)
	{
		IsRunning = false;
	}
	else
	{
		IsRunning = ResourcesComponent->CanConsumeResources(FResourcesStruct(0, RunStaminaCost*GetWorld()->GetDeltaSeconds(), 0)) && !bUseControllerRotationYaw;
	}
	SetWizardSpeed();
}

void ABaseWizard::EndDash()
{
	if (HasAuthority() && IsDashing)
	{
		IsDashing = false;
		SetupMovementType();
		PlayMontageBasedOnState();
	}
}

void ABaseWizard::EndGetHit()
{
	if (HasAuthority() && IsGettingHitHard())
	{
		SetWizardState(EWizardState::EWCS_Combat);
	}
}

void ABaseWizard::OnGetHitHard(const float DotProductDir)
{
	if (!IsGettingHitHard())
	{
		DeactivateItem(true);
		ItemsComponent->ClearSpellCombo();
		if (DotProductDir < 0)	//if came from front, character and damager has oposite directions
		{
			SetWizardState(EWizardState::EWCS_GettingHitFront);
		}
		else
		{
			SetWizardState(EWizardState::EWCS_GettingHitBack);
		}
		PlayMontageBasedOnState();
	}
}

uint32 ABaseWizard::GetWizardLevel()
{
	if (GetWizardPlayerState())
	{
		return WizardPlayerState->GetWizardLevel();
	}
	return 0;
}

bool ABaseWizard::IsGettingHitHard()
{
	return WizardState >= EWizardState::EWCS_GettingHitFront && WizardState <= EWizardState::EWCS_GettingHitBack;
}


void ABaseWizard::SetWizardState(const EWizardState WizardState_)
{
	if (WizardState != WizardState_)
	{
		WizardState = WizardState_;
		SetupMovementType();
	}
}

void ABaseWizard::SetupMovementType()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->SetMovementMode(WizardState == EWizardState::EWCS_FlyingBroom ? EMovementMode::MOVE_Flying : EMovementMode::MOVE_Walking);
	}

	bUseControllerRotationPitch = WizardState == EWizardState::EWCS_FlyingBroom;

	bUseControllerRotationYaw = 
		(WizardState == EWizardState::EWCS_DoingCombo) ||
		(IsActivatingItem() && WizardState != EWizardState::EWCS_DrinkingPotion) ||
		(WizardState == EWizardState::EWCS_FlyingBroom) ||
		(IsDashing);


	GetCharacterMovement()->bOrientRotationToMovement = !bUseControllerRotationYaw;
	SetWizardSpeed();
}

void ABaseWizard::SetWizardSpeed()
{
	if (WizardState == EWizardState::EWCS_FlyingBroom)
	{
		float Deceleration = 0;
		ItemsComponent->GetFlyingBroomStats(&MaxFlySpeed, &Deceleration);
		GetCharacterMovement()->MaxFlySpeed = MaxFlySpeed;
		GetCharacterMovement()->BrakingDecelerationFlying = Deceleration;
		GetCharacterMovement()->MaxAcceleration = MaxFlySpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = bUseControllerRotationYaw ? InCombatSpeed : IsRunning ? WalkSpeed * RunSpeedMult : WalkSpeed;
		GetCharacterMovement()->MaxAcceleration = 2048;
	}
}

float ABaseWizard::GetMaxSpeed()
{
	if (WizardState == EWizardState::EWCS_FlyingBroom)
	{
		return MaxFlySpeed;
	}
	return bUseControllerRotationYaw ? InCombatSpeed : WalkSpeed * RunSpeedMult;
}

uint32 ABaseWizard::GetCurrentItemHandHoldId()
{
	return ItemsComponent->GetHandHoldItemType();
}

void ABaseWizard::PlayMontageBasedOnState()
{
	if (WizardState == EWizardState::EWCS_ThrowingSpell)
	{
		PlayVoiceLineAttackMontage(true);
		PlayMontage(ThrowSpellMontage, 2, true);
	}
	else if (WizardState == EWizardState::EWCS_HoldingSpell)
	{
		PlayVoiceLineAttackMontage(true);
		PlayMontage(HoldSpellMontage, 1, false);
	}
	else if (WizardState == EWizardState::EWCS_ThrowingPotion)
	{
		PlayVoiceLineAttackMontage(false);
		PlayMontage(ThrowPotionMontage, 1, false);
	}
	else if (WizardState == EWizardState::EWCS_DrinkingPotion)
	{
		PlayMontage(DrinkPotionMontage, 1, false);
	}
	else if (WizardState == EWizardState::EWCS_ChangeItem_Base)
	{
		PlayMontage(ChangeItem_Basic, 1, false);
	}
	else if (WizardState == EWizardState::EWCS_ChangeItem_FlyingBroom)
	{
		PlayMontage(ChangeItem_FlyingBroom, 1, false);
	}
	else if (WizardState == EWizardState::EWCS_GettingHitFront)
	{
		PlayVoiceLinePainMontage();
		PlayMontage(GetHitHardMontage, 1, false);
	}
	else if (WizardState == EWizardState::EWCS_GettingHitBack)
	{
		PlayVoiceLinePainMontage();
		PlayMontage(GetHitHardMontage, 2, false);
	}
	else if (WizardState == EWizardState::EWCS_Dead)
	{
		PlayVoinceLineGrllsPain();
	}
}

void ABaseWizard::OnRepWizardState(EWizardState LastWizardState)
{
	if (LastWizardState == EWizardState::EWCS_HoldingSpell)
	{
		StopAnimMontage(HoldSpellMontage);
	}
	if (WizardState == EWizardState::EWCS_Dead)
	{
		OnWizardDead();
	}
	
	SetupMovementType();
	PlayMontageBasedOnState();
}

void ABaseWizard::OnRepIsRunning()
{
	SetupMovementType();
}

void ABaseWizard::OnRepIsDashing()
{
	SetupMovementType();
	if (IsDashing)
	{
		PlayMontage(DashMontage, 0, false, IsRunning ? .2f : 0);
	}
}

void ABaseWizard::OnRepComboDir(uint8 ComboDir) //called from items component not on rep...
{
	if (ComboDir != 0)
	{
		PlayComboMontage((ESpellComboDir)ComboDir);
	}
}