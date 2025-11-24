// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
//#include "GameTypes/UIInventoryEnums.h"
#include "GameTypes/InventoryStructs.h"
#include "WoDSaveGame.generated.h"


UCLASS()
class WOD_PROJECT_API UWoDSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UWoDSaveGame();

	TArray<FItemInventory> BagMWs = TArray<FItemInventory>{ FItemInventory(0)};
	TArray<FItemInventory> BankMWs;
	uint8 MWBagSlots = 1;
	uint8 MWBankSlots = 2;
	uint32 EquippedMW = 0;
	uint8 EquippedMWPerk = 0;

	TArray<FItemInventory> BagFBs;
	TArray<FItemInventory> BankFBs;
	uint8 FBsBagSlots = 1;
	uint8 FBsBankSlots = 2;
	uint32 EquippedFB = 0;
	uint8 EquippedFBPerk = 0;

	TMap<uint32, uint32> BagRelics;
	TMap<uint32, uint32> BankRelics;
	uint8 RelicsBagSlots = 1;
	uint8 RelicsBankSlots = 2;

	uint8 PotionsBagSlots = 1;
	uint8 GroceriesBagSlots = 1;

	TMap<uint32, uint32> MyCharacters;//code / level
	uint8 EquippedCharPerk = 0;
	uint32 EquippedChar = 0;

	TArray<uint32> MySpells = TArray<uint32>{0};
	TArray<uint32> MyPotionsRecepies;
	TArray<uint32> MyFields;	

	uint32 Gold = 0;
	uint32 Silver = 0;

	uint32 Experience = 0;
	uint32 Rank = 0;

	uint32 CurrentMission = 0;
	FVector_NetQuantize WorldSavePos;
};
