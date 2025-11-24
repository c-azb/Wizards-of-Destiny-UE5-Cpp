// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameTypes/WizardStates.h"

#include "WizardItemsComponent.generated.h"

class AHoldItem;
class APotion;
class AFlyingBroom;
class UDataTable;
struct FResourcesStruct;
struct FBoostParameters;

class ABaseWizard;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WOD_PROJECT_API UWizardItemsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWizardItemsComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	
	bool AddSpellCombo(const uint8 ComboDir);
	void ClearSpellCombo();
	//bool IsSpellComboLocallyValid();

	void ActivateItem();
	EActivateItem SetupItemActivation(const uint32 WizardLevel);
	void DeactivateItem(const bool bCancel = false, const int32 EquippedMW=0);

	void ChangeItem(TObjectPtr<USkeletalMeshComponent> WizardSkelleton);
	bool SetupChangeItem(const uint32 ItemId, const EHoldItem ItemType, const uint32 WizardLvl);

	FResourcesStruct GetItemCost(const bool bEquipCost);
	bool GetContinuousItemCost(FResourcesStruct& OutItemCost);
	FBoostParameters GetItemBoost();

	void GetFlyingBroomStats(float* FlySpeed, float* BreakDeceleration);
	bool CanUseFlyingBroom();
	void FlyingBroomDelayCounter(const float DeltaTime);

	uint32 GetHandHoldItemType();
	bool IsWizardLevelEnoughToEquip(const uint32 WizardLevel);

	EHoldItem GetCurrentItem();

	EActivateItem GetItemActivateType();

private:

	TObjectPtr<AHoldItem> PendingChangeItem=nullptr;
	uint32 PendingChangeItemId = 0;
	uint32 CurrentItemId = 0;
	
	UPROPERTY(Replicated)TObjectPtr<AHoldItem> CurrentItemInst=nullptr;
	//TObjectPtr<AFlyingBroom> FlyingBroom = nullptr;
	UPROPERTY(EditAnywhere, Category = "Att_Items") float FBDelay = 10;
	float CurrentFBDelay = 0;

	UPROPERTY(ReplicatedUsing=OnRep_SpellCombo) FString SpellCombination;
	UPROPERTY(EditAnywhere, Category = "Att_Items") float CombinationTime = 4;
	FTimerHandle ComboTimerHandle;

	UFUNCTION() void OnRep_SpellCombo();

	//UFUNCTION() void OnRep_CurrentItem();
	UFUNCTION() void SpellComboTimeOut();

	UClass* GetNewChangeItem(const uint32 ItemId, const EHoldItem ItemType);
	void DestroyCurrentItem();

	bool GetBaseWizard();
	TObjectPtr<ABaseWizard> BaseWizard=nullptr;
};
