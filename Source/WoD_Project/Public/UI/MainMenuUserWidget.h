// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTypes/MatchSetupEnums.h"
#include "MainMenuUserWidget.generated.h"

class UButton;
class UImage;
class UPanelWidget;
class UPlayPanelWidget;
class UConfirmPanelWidget;

UCLASS()
class WOD_PROJECT_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> WizardDuelBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> WizardWarBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ChalangesBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> InventoryBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> BagBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> CharacterBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> MagicWandBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> OptionsBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> RankImg;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> LvlImg;


	UPROPERTY(meta = (BindWidget)) TObjectPtr<UPlayPanelWidget> WBPPlayPanel;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UConfirmPanelWidget> WBPConfirmPanel;

	EPlayOption PlayOption = EPlayOption::EPO_None;

	UFUNCTION() void OnClickWizardDuel();
	UFUNCTION() void OnClickWizardWar();
	UFUNCTION() void OnClickChalanges();

	UFUNCTION() void OnClickPlay();

	UFUNCTION() void OnClickConfirmBtn();
};
