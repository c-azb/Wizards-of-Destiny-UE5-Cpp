// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GameTypes/UIInventoryEnums.h"
#include "GameTypes/WizardStates.h"

#include "WoDGameInstance.generated.h"

struct FBaseItem;
struct FItemInventory;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginEvent, const bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadPlayerDataEvent, const bool, bSuccess);

UCLASS()
class WOD_PROJECT_API UWoDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/*Login system*/
	FOnLoginEvent OnLoginEvent;
	void StartLogin();
	UFUNCTION() void OnLoggin(bool bSuccess);
	bool IsLoggedIn();

	/*Player cloud data*/
	FOnLoadPlayerDataEvent OnLoadPlayerDataEvent;
	void LoadPlayerData();
	UFUNCTION() void OnPlayerDataLoaded(bool bSuccess,bool bInicialize);

	void SavePlayerDataOnCloud(TArray<uint8>& SaveData, const FString& FileName);
	void SaveCurrentPlayerData();

	const FString PlayerDataFileName = TEXT("PlayerData");

	/*Player data*/
	void GetInventoryItems(const EInventoryType InventoryType, const EHoldItem ItemType, TArray<FItemInventory>& OutItems, uint8& SlotsCount);
	void GetInventoryItems(const EInventoryType InventoryType, const EHoldItem ItemType, TMap<uint32, uint32>& OutItems, uint8& SlotsCount);
	uint32 GetEquipedItem(const EHoldItem HoldItem);
	bool GetEquipedCharacter(uint32& CharacterId, uint32& CharacterLvl);

	void SetEquippedItem(const EHoldItem HoldItem, const uint32 ItemId);

	/*Items system*******************/
	FBaseItem* SearchItem(const uint32 ItemCode, const EHoldItem ItemType);
	UClass* SearchItemClass(const uint32 ItemCode, const EHoldItem ItemType);

	UClass* SearchSpellByCombo(const FString& SpellCombo);
	FBaseItem* SearchSpellById(const uint32 SpellId);

	UClass* SearchCharacterClass(const uint32 CharacterCode);
	FBaseItem* SearchCharacterData(const uint32 CharacterCode);

	FBaseItem* SearchDataTable(const FName& SearchId, TObjectPtr<UDataTable> ItemDataTable);

	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> MWDataTable;
	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> PotionsDataTable;
	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> FBsDataTable;
	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> SpellsDataTable;

	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> GrocerysDataTable;
	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> RelicsDataTable;

	UPROPERTY(EditAnywhere, Category = "Att_Items") TObjectPtr<UDataTable> CharactersDataTable;

};
