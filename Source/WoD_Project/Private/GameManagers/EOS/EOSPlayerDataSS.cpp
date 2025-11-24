// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/EOS/EOSPlayerDataSS.h"
#include "GameManagers/WoDSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameTypes/InventoryStructs.h"

bool UEOSPlayerDataSS::InicializePlayerData()
{
	if (PlayerData == nullptr)
	{
		PlayerData = Cast<UWoDSaveGame>(UGameplayStatics::CreateSaveGameObject(UWoDSaveGame::StaticClass()));
		return PlayerData != nullptr;
	}
	return false;
}

bool UEOSPlayerDataSS::GetPlayerData(TArray<uint8>& OutData)
{
	return UGameplayStatics::SaveGameToMemory(PlayerData, OutData);
}

bool UEOSPlayerDataSS::LoadPlayerData(TArray<uint8>& LoadData)
{
	if (PlayerData = Cast<UWoDSaveGame>(UGameplayStatics::LoadGameFromMemory(LoadData)))
	{
		return true;
	}
	return false;
}

void UEOSPlayerDataSS::GetInventoryItems(const EInventoryType InventoryType, const EHoldItem InventoryTab, TArray<FItemInventory>& OutItems, uint8& SlotsCount)
{
	/**DEBUG****/
	if (!PlayerData)
	{
		InicializePlayerData();
	}

	/******/
	if (!PlayerData) return;

	if (InventoryTab == EHoldItem::EHI_MagicWand)
	{
		OutItems = InventoryType == EInventoryType::EIT_Bag ? PlayerData->BagMWs : PlayerData->BankMWs;
		SlotsCount = InventoryType == EInventoryType::EIT_Bag ? PlayerData->MWBagSlots : PlayerData->MWBankSlots;
	}
	else if (InventoryTab == EHoldItem::EHI_FlyingBroom)
	{
		OutItems = InventoryType == EInventoryType::EIT_Bag ? PlayerData->BagFBs : PlayerData->BankFBs;
		SlotsCount = InventoryType == EInventoryType::EIT_Bag ? PlayerData->FBsBagSlots : PlayerData->FBsBankSlots;
	}
}

void UEOSPlayerDataSS::GetInventoryItems(const EInventoryType InventoryType, const EHoldItem InventoryTab, TMap<uint32, uint32>& OutItems, uint8& SlotsCount)
{
	/**DEBUG****/
	if (!PlayerData)
	{
		InicializePlayerData();
	}
	/******/
	if (!PlayerData) return;

	/*if (InventoryTab == EHoldItem::EHI_Potion)
	{
		OutItems = PlayerData->BagPotions;
		SlotsCount = PlayerData->PotionsBagSlots;
	}
	else if (InventoryTab == EHoldItem::EHI_Grocery)
	{
		OutItems = PlayerData->BagGroceries;
		SlotsCount = PlayerData->GroceriesBagSlots;
	}
	else*/ if (InventoryTab == EHoldItem::EHI_Relics)
	{
		OutItems = InventoryType == EInventoryType::EIT_Bag ? PlayerData->BagRelics: PlayerData->BankRelics;
		SlotsCount = InventoryType == EInventoryType::EIT_Bag ? PlayerData->RelicsBagSlots : PlayerData->RelicsBankSlots;
	}
}

TMap<uint32, uint32>* UEOSPlayerDataSS::GetAllCharacters()
{
	if (PlayerData)
	{
		return &PlayerData->MyCharacters;
	}

	return nullptr;
}

bool UEOSPlayerDataSS::GetEquipedCharacter(uint32& OutCodeId, uint32& Lvl)
{
	if (PlayerData)
	{
		TMap<uint32, uint32>* AllCharacters = GetAllCharacters();
		OutCodeId = PlayerData->EquippedChar;
		for (const TPair<uint32, uint32>& Character : *AllCharacters)
		{
			if (Character.Key == OutCodeId)
			{
				Lvl = Character.Value;
				return true;
			}
		}
	}
	return false;
}

uint32 UEOSPlayerDataSS::GetEquipedItem(const EHoldItem HoldItem)
{
	if (!PlayerData) return 0;

	if (HoldItem == EHoldItem::EHI_MagicWand)
	{
		return PlayerData->EquippedMW;
	}
	else if (HoldItem == EHoldItem::EHI_FlyingBroom)
	{
		return PlayerData->EquippedFB;
	}
	/*else if (HoldItem == EHoldItem::EHI_Potion)
	{
		return PlayerData->EquippedPotion;
	}*/

	return 0;
}

void UEOSPlayerDataSS::SetEquippedItem(const EHoldItem HoldItem, const uint32 ItemId)
{
	if (HoldItem == EHoldItem::EHI_MagicWand)
	{
		PlayerData->EquippedMW = ItemId;
	}
	else if (HoldItem == EHoldItem::EHI_FlyingBroom)
	{
		PlayerData->EquippedFB = ItemId;
	}
}
