// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/WoDGameInstance.h"
#include "GameManagers/EOS/EOSLoginSS.h"
#include "GameManagers/EOS/EOSUserCloudSS.h"
#include "GameManagers/EOS/EOSPlayerDataSS.h"

#include "GameTypes/BaseItemStruct.h"


void UWoDGameInstance::StartLogin()
{
	UEOSLoginSS* LoginSS = GetSubsystem<UEOSLoginSS>();
	if (LoginSS)
	{
		LoginSS->OnLogin.Clear();
		LoginSS->OnLogin.AddDynamic(this, &ThisClass::OnLoggin);
		LoginSS->Login();
	}
}

void UWoDGameInstance::OnLoggin(bool bSuccess)
{
	if (bSuccess)
	{
		LoadPlayerData();
	}
	OnLoginEvent.Broadcast(bSuccess);
}

bool UWoDGameInstance::IsLoggedIn()
{
	UEOSLoginSS* LoginSS = GetSubsystem<UEOSLoginSS>();
	if (LoginSS)
	{
		return LoginSS->IsLoggedIn();
	}
	return false;
}

void UWoDGameInstance::LoadPlayerData()
{
	if (!IsLoggedIn()) return;
	UEOSUserCloudSS* UserCloudSS = GetSubsystem<UEOSUserCloudSS>();
	if (UserCloudSS)
	{
		UserCloudSS->OnLoadData.Clear();
		UserCloudSS->OnLoadData.AddDynamic(this, &ThisClass::OnPlayerDataLoaded);
		UserCloudSS->LoadPlayerData();
	}
}

void UWoDGameInstance::OnPlayerDataLoaded(bool bSuccess, bool bInicialize)
{
	//move to data storage subsysyem
	if (!IsLoggedIn()) return;

	if (bSuccess && !bInicialize)
	{
		TArray<uint8>* LoadedPlayerData=nullptr;
		if (GetSubsystem<UEOSUserCloudSS>()->GetLoadedData(&LoadedPlayerData))
		{
			if (GetSubsystem<UEOSPlayerDataSS>()->LoadPlayerData(*LoadedPlayerData))
			{
				//data is available in the subsystem
			}
		}
	}
	else if (bSuccess && bInicialize)
	{
		if (GetSubsystem<UEOSPlayerDataSS>()->InicializePlayerData())
		{
			SaveCurrentPlayerData();
		}
	}
	OnLoadPlayerDataEvent.Broadcast(bSuccess);
}

void UWoDGameInstance::SaveCurrentPlayerData()
{
	TArray<uint8> InitialData;
	if (GetSubsystem<UEOSPlayerDataSS>()->GetPlayerData(InitialData))
	{
		SavePlayerDataOnCloud(InitialData, PlayerDataFileName);
	}
}


void UWoDGameInstance::SavePlayerDataOnCloud(TArray<uint8>& SaveData,const FString& FileName)
{
	GetSubsystem<UEOSUserCloudSS>()->SavePlayerData(SaveData, FileName);
}

void UWoDGameInstance::GetInventoryItems(const EInventoryType InventoryType, const EHoldItem ItemType, TArray<FItemInventory>& OutItems, uint8& SlotsCount)
{
	GetSubsystem<UEOSPlayerDataSS>()->GetInventoryItems(InventoryType, ItemType, OutItems, SlotsCount);
}
void UWoDGameInstance::GetInventoryItems(const EInventoryType InventoryType, const EHoldItem ItemType, TMap<uint32, uint32>& OutItems, uint8& SlotsCount)
{
	GetSubsystem<UEOSPlayerDataSS>()->GetInventoryItems(InventoryType, ItemType, OutItems, SlotsCount);
}

uint32 UWoDGameInstance::GetEquipedItem(const EHoldItem HoldItem)
{
	return GetSubsystem<UEOSPlayerDataSS>()->GetEquipedItem(HoldItem);
}

void UWoDGameInstance::SetEquippedItem(const EHoldItem HoldItem, const uint32 ItemId)
{
	GetSubsystem<UEOSPlayerDataSS>()->SetEquippedItem(HoldItem,ItemId);
}

bool UWoDGameInstance::GetEquipedCharacter(uint32& CharacterId, uint32& CharacterLvl)
{
	return GetSubsystem<UEOSPlayerDataSS>()->GetEquipedCharacter(CharacterId, CharacterLvl);
}

/*********************/
FBaseItem* UWoDGameInstance::SearchItem(const uint32 ItemCode, const EHoldItem ItemType)
{
	FName Code_String = FName(FString::Printf(TEXT("%d"), ItemCode));

	if (ItemType == EHoldItem::EHI_MagicWand)
	{
		return SearchDataTable(Code_String, MWDataTable);
	}
	else if (ItemType == EHoldItem::EHI_FlyingBroom)
	{
		return SearchDataTable(Code_String, FBsDataTable);
	}
	else if (ItemType == EHoldItem::EHI_Potion)
	{
		return SearchDataTable(Code_String, PotionsDataTable);
	}
	else if (ItemType == EHoldItem::EHI_Grocery)
	{
		return SearchDataTable(Code_String, GrocerysDataTable);
	}
	else if (ItemType == EHoldItem::EHI_Relics)
	{
		return SearchDataTable(Code_String, RelicsDataTable);
	}
	return nullptr;
}

UClass* UWoDGameInstance::SearchItemClass(const uint32 ItemCode, const EHoldItem ItemType)
{
	if (FBaseItem* BaseItem = SearchItem(ItemCode, ItemType))
	{
		return BaseItem->ItemClass;
	}
	return nullptr;
}

UClass* UWoDGameInstance::SearchSpellByCombo(const FString& SpellCombo)
{
	if (FBaseItem* BaseItem = SearchDataTable(FName(SpellCombo), SpellsDataTable))
	{
		return BaseItem->ItemClass;
	}
	return nullptr;
}

FBaseItem* UWoDGameInstance::SearchSpellById(const uint32 SpellId)
{
	TArray<FName> RowNames = SpellsDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		if (FBaseItem* BaseItem = SpellsDataTable->FindRow<FBaseItem>(RowName, FString()))
		{
			if (FSpellData* SpellData = static_cast<FSpellData*>(BaseItem))
			{
				if (SpellData->SpellId == SpellId)
				{
					return BaseItem;
				}
			}
		}
	}
	return nullptr;
}

UClass* UWoDGameInstance::SearchCharacterClass(const uint32 CharacterCode)
{
	if (FBaseItem* BaseItem = SearchCharacterData(CharacterCode))
	{
		return BaseItem->ItemClass;
	}
	return nullptr;
}

FBaseItem* UWoDGameInstance::SearchCharacterData(const uint32 CharacterCode)
{
	FName Code_String = FName(FString::Printf(TEXT("%d"), CharacterCode));
	return SearchDataTable(Code_String, CharactersDataTable);
}

FBaseItem* UWoDGameInstance::SearchDataTable(const FName& RowName, TObjectPtr<UDataTable> ItemDataTable)
{
	if (ItemDataTable)
	{
		return ItemDataTable->FindRow<FBaseItem>(RowName, FString());
	}
	return nullptr;
}