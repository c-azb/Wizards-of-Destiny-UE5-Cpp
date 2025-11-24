// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"


class UProgressBar;
class UTextBlock;
struct FResourcesStruct;

UCLASS()
class WOD_PROJECT_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetResourcesBars(const FResourcesStruct& Amount, const FResourcesStruct& MaxAmount, const float Health, const float MaxHealth);
	void SetWizardLevelBars(const uint32 WizardLevel, const float WizardScore);

private:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UProgressBar> WizardLvlBar;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> WizardLvLTxt;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ManaAmountTxt;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> SpecialAmountTxt;

	void SetBar(const float Amount, const float MaxAmount, TObjectPtr<UProgressBar> BarRef);
	void SetTextAmount(const float Amount, const float MaxAmount, TObjectPtr<UTextBlock> TxtRef);

};
