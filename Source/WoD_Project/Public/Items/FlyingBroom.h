// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/HoldItem.h"
#include "FlyingBroom.generated.h"

class UNiagaraComponent;

UCLASS()
class WOD_PROJECT_API AFlyingBroom : public AHoldItem
{
	GENERATED_BODY()

public:
	AFlyingBroom();

	UPROPERTY(EditAnywhere, Category = "Att_FlyingBroom") float MaxSpeed=1000;
	//UPROPERTY(EditAnywhere, Category = "Att_FlyingBroom") float MaxFuel=100;
	//UPROPERTY(EditAnywhere, Category = "Att_FlyingBroom") float FuelConsume=3;
	UPROPERTY(EditAnywhere, Category = "Att_FlyingBroom") float BreakDeceleration=10;
	UPROPERTY(EditAnywhere, Category = "Att_FlyingBroom") float StaminaConsume=1;

	virtual EHoldItem GetItemType() override;
	virtual FResourcesStruct GetItemCost(const bool bEquip) override;
	virtual bool GetContinuousItemCost(FResourcesStruct& ContinuousCost) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> FBNiagaraComponent;

	UPROPERTY(EditAnywhere, Category = "Att_Item")FResourcesStruct EquipItemCost;
	//UPROPERTY(EditAnywhere, Category = "Att_FB") TObjectPtr<UAnimationAsset> EquipFBAnim;
	UPROPERTY(EditAnywhere, Category = "Att_FB") TObjectPtr<UAnimMontage> EquipFBMontage;
};
