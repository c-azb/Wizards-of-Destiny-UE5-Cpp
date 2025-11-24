// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Potions/Potion.h"
#include "DrinkPotion.generated.h"

/**
 * 
 */
UCLASS()
class WOD_PROJECT_API ADrinkPotion : public APotion
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Att_DrinkPotion") FBoostParameters BoostParameters;
	virtual FBoostParameters GetItemBoost() override;
	virtual bool ActivateItem() override;
	
protected:
	virtual EActivateItem GetActivateType() override;

};
