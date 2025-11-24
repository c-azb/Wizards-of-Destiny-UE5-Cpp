// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/WizardItemsComponent.h"
#include "Items/MagicWand.h"
#include "Items/Potions/Potion.h"
#include "Items/FlyingBroom.h"

#include "GameTypes/ResourcesStruct.h"
#include "GameManagers/WoDGameInstance.h"

#include "Net/UnrealNetwork.h"

#include "Characters/Wizard/BaseWizard.h"

UWizardItemsComponent::UWizardItemsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWizardItemsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWizardItemsComponent,SpellCombination);
	DOREPLIFETIME(UWizardItemsComponent,CurrentItemInst);
}


void UWizardItemsComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UWizardItemsComponent::AddSpellCombo(const uint8 ComboDir)
{
	if (GetCurrentItem() == EHoldItem::EHI_MagicWand && !CurrentItemInst->IsItemReady())
	{
		if (SpellCombination.IsEmpty())
		{
			GetOwner()->GetWorldTimerManager().
				SetTimer(ComboTimerHandle, this, &ThisClass::SpellComboTimeOut, CombinationTime);
		}
		SpellCombination.AppendChar(ComboDir);
		return true;
	}
	return false;
}

void UWizardItemsComponent::OnRep_SpellCombo()
{
	uint8 LastComboDir = 0;
	if (!SpellCombination.IsEmpty())
	{
		LastComboDir = SpellCombination[SpellCombination.Len() - 1];
	}

	if (GetBaseWizard())
	{
		BaseWizard->OnRepComboDir(LastComboDir);
	}
}

void UWizardItemsComponent::SpellComboTimeOut()
{
	ClearSpellCombo();

	if (GetBaseWizard())
	{
		const bool bActivate = GetCurrentItem() == EHoldItem::EHI_MagicWand && CurrentItemInst->IsItemReady();
		BaseWizard->OnSpellComboTimeout(bActivate);
	}
}

void UWizardItemsComponent::ClearSpellCombo()
{
	SpellCombination.Empty();
	if (ComboTimerHandle.IsValid())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(ComboTimerHandle);
	}
}

/*bool UWizardItemsComponent::IsSpellComboLocallyValid()
{
	return GetCurrentItem() == EHoldItem::EHI_MagicWand ? !SpellCombination.IsEmpty() : true;
}*/

EActivateItem UWizardItemsComponent::SetupItemActivation(const uint32 WizardLevel)
{
	EHoldItem CurrentItem = GetCurrentItem();
	if (CurrentItem == EHoldItem::EHI_MagicWand)
	{
		UWoDGameInstance* WoDGameInstance = GetWorld()->GetGameInstance<UWoDGameInstance>();
		if (UClass* SpellClass = WoDGameInstance->SearchSpellByCombo(SpellCombination))
		{
			if (AMagicWand* MagicWand = Cast<AMagicWand>(CurrentItemInst))
			{
				if (MagicWand->SetupSpell(SpellClass, WizardLevel))
				{
					return MagicWand->GetActivateType();
				}
			}
		}
	}
	else if (CurrentItem == EHoldItem::EHI_Potion)
	{
		//check enougth special
		return CurrentItemInst->GetActivateType();
	}

	return EActivateItem::EAI_None;
}

FResourcesStruct UWizardItemsComponent::GetItemCost(const bool bEquipCost)
{
	if (bEquipCost && PendingChangeItem)
	{
		return PendingChangeItem->GetItemCost(bEquipCost);
	}
	else if (CurrentItemInst)
	{
		return CurrentItemInst->GetItemCost(bEquipCost);
	}
	return FResourcesStruct();
}

bool UWizardItemsComponent::GetContinuousItemCost(FResourcesStruct& OutItemCost)
{
	if (CurrentItemInst)
	{
		return CurrentItemInst->GetContinuousItemCost(OutItemCost);
	}
	return false;
}

FBoostParameters UWizardItemsComponent::GetItemBoost()
{
	if (CurrentItemInst)
	{
		return CurrentItemInst->GetItemBoost();
	}
	return FBoostParameters();
}

EHoldItem UWizardItemsComponent::GetCurrentItem()
{
	if (CurrentItemInst)
	{
		return CurrentItemInst->GetItemType();
	}

	return EHoldItem::EHI_None;
}

EActivateItem UWizardItemsComponent::GetItemActivateType()
{
	if (CurrentItemInst)
	{
		return CurrentItemInst->GetActivateType();
	}
	return EActivateItem::EAI_None;
}

void UWizardItemsComponent::ActivateItem()
{
	CurrentItemInst->ActivateItem();
}

void UWizardItemsComponent::DeactivateItem(const bool bCancel,const int32 EquippedMW)
{
	if (GetOwner()->HasAuthority())
	{
		if (CurrentItemInst) 
		{
			CurrentItemInst->DeactivateItem();
		}
		if (GetCurrentItem() == EHoldItem::EHI_Potion && !bCancel)
		{
			if (CurrentItemInst->GetActivateType() == EActivateItem::EAI_ThrowPotion)
			{
				CurrentItemInst = nullptr;
			}
			if (GetBaseWizard())
			{
				//hard coded default item , needs to get equiped MW code (set global?)
				BaseWizard->StartChangeItem(EquippedMW, EHoldItem::EHI_MagicWand);
			}
		}
	}
}

bool UWizardItemsComponent::SetupChangeItem(const uint32 ItemId, const EHoldItem ItemType,const uint32 WizardLvl)
{
	if (UClass* NewItemClass = GetNewChangeItem(ItemId, ItemType))
	{
		if (PendingChangeItem = Cast<AHoldItem>(NewItemClass->GetDefaultObject()))
		{
			if (PendingChangeItem->GetItemType() == EHoldItem::EHI_FlyingBroom)
			{
				if (!CanUseFlyingBroom())
				{
					PendingChangeItem = nullptr;
					return false;
				}
				CurrentFBDelay = FBDelay;
			}

			if (IsWizardLevelEnoughToEquip(WizardLvl))
			{
				PendingChangeItemId = ItemId;
				return true;
			}
		}
	}

	PendingChangeItem = nullptr;
	return false;
}

void UWizardItemsComponent::ChangeItem(TObjectPtr<USkeletalMeshComponent> WizardSkelleton)
{
	if (PendingChangeItem)
	{
		AHoldItem* NewItem = nullptr;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = SpawnParameters.Instigator = Cast<APawn>(GetOwner());
		NewItem = GetWorld()->SpawnActor<AHoldItem>(PendingChangeItem->GetClass(), FTransform(), SpawnParameters);
		PendingChangeItem = nullptr;

		if (NewItem)
		{
			DestroyCurrentItem();
			NewItem->AttachHoldItem(WizardSkelleton);
			CurrentItemInst = NewItem;
			CurrentItemId = PendingChangeItemId;
		}
	}
}

UClass* UWizardItemsComponent::GetNewChangeItem(const uint32 ItemId, const EHoldItem ItemType)
{
	if (ItemId == CurrentItemId && ItemType == GetCurrentItem())
	{
		return nullptr;
	}

	UWoDGameInstance* WoDGameInstance = GetWorld()->GetGameInstance<UWoDGameInstance>();
	return WoDGameInstance->SearchItemClass(ItemId, ItemType);
}



void UWizardItemsComponent::GetFlyingBroomStats(float* FlySpeed, float* BreakDeceleration)
{
	if (AFlyingBroom* FlyingBroom = Cast<AFlyingBroom>(CurrentItemInst))
	{
		*FlySpeed = FlyingBroom->MaxSpeed;
		*BreakDeceleration = FlyingBroom->BreakDeceleration;
	}
}

bool UWizardItemsComponent::CanUseFlyingBroom()
{
	return CurrentFBDelay <= 0;
}

void UWizardItemsComponent::FlyingBroomDelayCounter(const float DeltaTime)
{
	if (GetCurrentItem() != EHoldItem::EHI_FlyingBroom && CurrentFBDelay > 0)
	{
		CurrentFBDelay = FMath::Clamp(CurrentFBDelay - DeltaTime, 0, FBDelay);
	}
}

uint32 UWizardItemsComponent::GetHandHoldItemType()
{
	if (CurrentItemInst)
	{
		return CurrentItemInst->HandHoldItemId;
	}
	return 0;
}

bool UWizardItemsComponent::IsWizardLevelEnoughToEquip(const uint32 WizardLevel)
{
	if (PendingChangeItem)
	{
		return PendingChangeItem->MinWizardLevel <= WizardLevel;
	}
	return false;
}



void UWizardItemsComponent::DestroyCurrentItem()
{
	if (CurrentItemInst)
	{
		CurrentItemInst->Destroy();
		CurrentItemInst = nullptr;
	}
}

bool UWizardItemsComponent::GetBaseWizard()
{
	if (BaseWizard == nullptr)
	{
		BaseWizard = Cast<ABaseWizard>(GetOwner());
	}
	return BaseWizard != nullptr;
}








