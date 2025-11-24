// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/HoldItem.h"
#include "MagicWand.generated.h"

class ASpell;

UCLASS()
class WOD_PROJECT_API AMagicWand : public AHoldItem
{
	GENERATED_BODY()

public:
	AMagicWand();
	
public:
	UPROPERTY(VisibleAnywhere) TObjectPtr<USceneComponent> WandPoint;

	virtual bool ActivateItem() override;
	virtual void DeactivateItem() override;
	virtual FResourcesStruct GetItemCost(const bool bEquip) override;
	virtual bool GetContinuousItemCost(FResourcesStruct& OutItemCost) override;

	virtual EActivateItem GetActivateType() override;
	virtual EHoldItem GetItemType() override;
	virtual bool IsItemReady();

	bool GetAimTargetPos(const float MaxDistance, const float CurrentReach, FHitResult* OutHitResult, FRotator* OutRot, FVector* OutHitPoint);
	bool SetupSpell(UClass* SpellClass,const uint32 CurrentWizardLvl);

private:
	UPROPERTY(EditAnywhere, Category = "Att_Item")FResourcesStruct EquipItemCost;
	TObjectPtr<ASpell> PendingSpell=nullptr;
	TObjectPtr<ASpell> CurrentSpellInst=nullptr;
};
