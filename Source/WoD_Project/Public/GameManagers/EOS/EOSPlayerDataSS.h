// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTypes/UIInventoryEnums.h"
#include "GameTypes/WizardStates.h"
#include "EOSPlayerDataSS.generated.h"

class UWoDSaveGame;
struct FItemInventory;

UCLASS()
class WOD_PROJECT_API UEOSPlayerDataSS : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	bool InicializePlayerData();
	bool GetPlayerData(TArray<uint8>& OutData);
	bool LoadPlayerData(TArray<uint8>& LoadData);

	void GetInventoryItems(const EInventoryType InventoryType, const EHoldItem InventoryTab, TArray<FItemInventory>& OutItems, uint8& SlotsCount);
	void GetInventoryItems(const EInventoryType InventoryType, const EHoldItem InventoryTab, TMap<uint32, uint32>& OutItems, uint8& SlotsCount);

	TMap<uint32, uint32>* GetAllCharacters();
	bool GetEquipedCharacter(uint32& OutCodeId,uint32& Lvl);

	uint32 GetEquipedItem(const EHoldItem HoldItem);

	void SetEquippedItem(const EHoldItem HoldItem, const uint32 ItemId);

private:

	TObjectPtr<UWoDSaveGame> PlayerData = nullptr;
	
};
