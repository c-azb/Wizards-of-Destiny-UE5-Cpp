// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/WizardPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Characters/Wizard/WizardPlayerController.h"


void AWizardPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWizardPlayerState, WizardKills);
	DOREPLIFETIME(AWizardPlayerState, Deaths);
	DOREPLIFETIME(AWizardPlayerState, WizardLevel);
	DOREPLIFETIME(AWizardPlayerState, Potions);
	DOREPLIFETIME(AWizardPlayerState, Grocerys);
	DOREPLIFETIME(AWizardPlayerState, Relics);
}

void AWizardPlayerState::AddScore(const EWizardScore WizardScore,const float Mult)
{
	AddKillDeathStats(WizardScore);

	if (WizardLevel >= MaxLevel)
	{
		return;
	}

	float NewScore = GetScore() + GetScoreAmount(WizardScore) * Mult;
	float PointsToNextLevel = PointsPerLevel * (WizardLevel + 1);

	if (GetScore() >= PointsToNextLevel)
	{
		WizardLevel++;

		float MaximunNewScore = PointsPerLevel * (WizardLevel + 1);
		NewScore = FMath::Clamp(GetScore() - PointsToNextLevel,0,MaximunNewScore - 1);

		if (WizardLevel >= MaxLevel)
		{
			NewScore = MaximunNewScore;
		}
	}

	SetScore(NewScore);
	OnRepWizardScores();
}

void AWizardPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	OnRepWizardScores();
}

void AWizardPlayerState::BeginPlay()
{
	Super::BeginPlay();
	OnRepWizardScores();
}

void AWizardPlayerState::AddKillDeathStats(const EWizardScore WizardScore)
{
	if (WizardScore == EWizardScore::EWS_Wizard_Kill)
	{
		WizardKills++;
	}
	else if (WizardScore == EWizardScore::EWS_Dead)
	{
		Deaths++;
	}
}

float AWizardPlayerState::GetScoreAmount(const EWizardScore WizardScore)
{
	if (WizardScore == EWizardScore::EWS_Wizard_Kill)
	{
		return 200;
	}
	else if (WizardScore == EWizardScore::EWS_Monster_Kill)
	{
		return 25;
	}
	else if (WizardScore == EWizardScore::EWS_SpellActivation)
	{
		return 5;
	}
	else if (WizardScore == EWizardScore::EWS_Pickup)
	{
		return 10;
	}
	else if (WizardScore == EWizardScore::EWS_Dead)
	{
		return 50;
	}
	return 0.0f;
}

void AWizardPlayerState::OnRepWizardScores()
{
	if (GetWizardController())
	{
		WizardController->OnUpdateScore(WizardLevel, GetScore() / (PointsPerLevel * (WizardLevel + 1)));
	}
}

bool AWizardPlayerState::GetWizardController()
{
	if (WizardController == nullptr)
	{
		WizardController = Cast<AWizardPlayerController>(GetPlayerController());
	}

	return WizardController != nullptr;
}

TArray<FBagInfo> AWizardPlayerState::GetPlayerItems(const EHoldItem HoldItem) const
{
	if (HoldItem == EHoldItem::EHI_Grocery)
	{
		return Grocerys;
	}
	else if (HoldItem == EHoldItem::EHI_Potion)
	{
		return Potions;
	}
	else if (HoldItem == EHoldItem::EHI_Relics)
	{
		return Relics;
	}
	return TArray<FBagInfo>();
}

int32 AWizardPlayerState::GetEquippedItem(const EHoldItem ItemType) const
{
	if (ItemType == EHoldItem::EHI_MagicWand)
	{
		return EquippedMW;
	}
	else if (ItemType == EHoldItem::EHI_FlyingBroom)
	{
		return EquippedFB;
	}
	else if (ItemType == EHoldItem::EHI_Potion)
	{
		return EquippedPotion;
	}
	return 0;
}

void AWizardPlayerState::SetEquippedItem(const EHoldItem ItemType,const int32 ItemId)
{
	if (ItemType == EHoldItem::EHI_MagicWand)
	{
		EquippedMW = ItemId;
	 }
	else if (ItemType == EHoldItem::EHI_FlyingBroom)
	{
		EquippedFB = ItemId;
	}
	else if (ItemType == EHoldItem::EHI_Potion)
	{
		EquippedPotion = ItemId;
	}
}

void AWizardPlayerState::AddPotion(const uint32 PotionId)
{
	AddNewItem(Potions, PotionId);
}

void AWizardPlayerState::AddPickUp(const uint32 PickUpId, const EPickUpType PickUpType)
{
	if (PickUpType == EPickUpType::EPUT_Grocery)
	{
		AddNewItem(Grocerys, PickUpId);
	}
	else if (PickUpType == EPickUpType::EPUT_Relic)
	{
		AddNewItem(Relics, PickUpId);
	}
}

void AWizardPlayerState::AddNewItem(TArray<FBagInfo>& ItemsArray, uint32 ItemId)
{
	for (int i = 0; i < ItemsArray.Num(); i++)
	{
		if (ItemsArray[i].ItemId == ItemId)
		{
			ItemsArray[i].ItemAmount++;
			return;
		}
	}

	FBagInfo BagInfo;
	BagInfo.ItemAmount = 1;
	BagInfo.ItemId = ItemId;

	ItemsArray.Add(BagInfo);
}
