// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes/WizardStates.h"
#include "GameTypes/ResourcesStruct.h"
#include "HoldItem.generated.h"


UCLASS()
class WOD_PROJECT_API AHoldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AHoldItem();
	void AttachHoldItem(TObjectPtr<USkeletalMeshComponent> HoldMesh);
	UFUNCTION() virtual bool ActivateItem();
	UFUNCTION() virtual bool ActivateItemCustom(const float DamageMult, const float DamageRangeMult);
	UFUNCTION() virtual void DeactivateItem();
	UFUNCTION() virtual FResourcesStruct GetItemCost(const bool bEquip);
	UFUNCTION() virtual bool GetContinuousItemCost(FResourcesStruct& OutItemCost);
	UFUNCTION() virtual EActivateItem GetActivateType();
	UFUNCTION() virtual EHoldItem GetItemType();
	UFUNCTION() virtual FBoostParameters GetItemBoost();
	UFUNCTION() virtual bool IsItemReady();

	UPROPERTY(EditAnywhere, Category = "Att_HoldItem") uint32 HandHoldItemId = 0;
	UPROPERTY(EditAnywhere, Category = "Att_HoldItem") uint32 MinWizardLevel = 0;

protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<USkeletalMeshComponent> ItemMesh;

};
