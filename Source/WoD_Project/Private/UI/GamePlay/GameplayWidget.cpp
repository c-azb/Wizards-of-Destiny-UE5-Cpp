// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GamePlay/GameplayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameTypes/ResourcesStruct.h"

void UGameplayWidget::SetResourcesBars(const FResourcesStruct& Amount, const FResourcesStruct& MaxAmount,const float Health, const float MaxHealth)
{
	SetBar(Health,MaxHealth, HealthBar);
	SetBar(Amount.Stamina, MaxAmount.Stamina, StaminaBar);
	SetTextAmount(Amount.Mana, MaxAmount.Mana, ManaAmountTxt);
	SetTextAmount(Amount.Special, MaxAmount.Special, SpecialAmountTxt);
}

void UGameplayWidget::SetWizardLevelBars(const uint32 WizardLevel, const float WizardScorePercent)
{
	WizardLvLTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), WizardLevel)));
	WizardLvlBar->SetPercent(WizardScorePercent);
}

void UGameplayWidget::SetBar(const float Amount, const float MaxAmount, TObjectPtr<UProgressBar> BarRef)
{
	BarRef->SetPercent(Amount / MaxAmount);
}

void UGameplayWidget::SetTextAmount(const float Amount, const float MaxAmount, TObjectPtr<UTextBlock> TxtRef)
{
	FText TextValue = FText::FromString(FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Amount), FMath::CeilToInt(MaxAmount)));
	TxtRef->SetText(TextValue);
}
