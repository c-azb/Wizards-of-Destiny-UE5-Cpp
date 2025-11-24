// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/HoldItem.h"
#include "Potion.generated.h"


class UProjectileMovementComponent;

UCLASS()
class WOD_PROJECT_API APotion : public AHoldItem
{
	GENERATED_BODY()

public:
	APotion();
	virtual FResourcesStruct GetItemCost(const bool bEquip) override;
	virtual EHoldItem GetItemType() override;
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Att_Potion") TObjectPtr<UClass> PotionTriggerEffect;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> StopperMesh;

private:
	UPROPERTY(EditAnywhere, Category = "Att_Item")FResourcesStruct ActivatePotionCost;
	UPROPERTY(EditAnywhere, Category = "Att_Potion") TObjectPtr<USoundBase> PickPotionSFX;


};
