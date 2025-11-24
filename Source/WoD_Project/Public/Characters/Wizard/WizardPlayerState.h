// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameTypes/WizardStates.h"
#include "GameTypes/PickUpEnums.h"
#include "WizardPlayerState.generated.h"

class AWizardPlayerController;

USTRUCT()
struct FBagInfo
{
	GENERATED_BODY()
public:
	uint8 ItemId=0;
	uint8 ItemAmount=1;
};

UCLASS()
class WOD_PROJECT_API AWizardPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddScore(const EWizardScore WizardScore, const float Mult);
	virtual void OnRep_Score() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated) uint32 WizardKills = 0;
	UPROPERTY(Replicated) uint32 Deaths = 0;
	UPROPERTY(ReplicatedUsing = OnRepWizardScores) uint32 WizardLevel = 0;

	UPROPERTY(Replicated) TArray<FBagInfo> Potions;
	UPROPERTY(Replicated) TArray<FBagInfo> Grocerys;
	UPROPERTY(Replicated) TArray<FBagInfo> Relics;  //store on data base on mach end

	//set server only on receive RPC change item from client
	int32 EquippedPotion = -1;
	int32 EquippedMW = 0;
	int32 EquippedFB = 0;

	AWizardPlayerController* WizardController = nullptr;

	const uint32 MaxLevel = 32;
	const float PointsPerLevel = 100;

	void AddKillDeathStats(const EWizardScore WizardScore);
	float GetScoreAmount(const EWizardScore WizardScore);

	UFUNCTION() void OnRepWizardScores();

	bool GetWizardController();

	void AddNewItem(TArray<FBagInfo>& ItemsArray, uint32 ItemId);

public:
	FORCEINLINE uint32 GetWizardKills() const { return WizardKills; }
	FORCEINLINE uint32 GetWizardDeaths() const { return Deaths; }
	FORCEINLINE uint32 GetWizardLevel() const { return WizardLevel; }

	FORCEINLINE TArray<FBagInfo> GetPlayerItems(const EHoldItem HoldItem) const;

	FORCEINLINE int32 GetEquippedItem(const EHoldItem ItemType) const;
	void SetEquippedItem(const EHoldItem ItemType, const int32 ItemId);
	void AddPotion(const uint32 PotionId);
	void AddPickUp(const uint32 PickUpId, const EPickUpType PickUpType);
};
