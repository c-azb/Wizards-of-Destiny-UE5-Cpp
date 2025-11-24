// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTypes/MatchSetupEnums.h"
#include "PlayPanelWidget.generated.h"

class UButton;
class UImage;
class UPanelWidget;
class UComboBoxString;
class UCheckBox;

UCLASS()
class WOD_PROJECT_API UPlayPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(meta = (BindWidget)) TObjectPtr<UPanelWidget> PlayModesBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ClosePlayPanelBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> MapImg;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UComboBoxString> MapPickComboBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> PlayBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> v1CheckBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> v2CheckBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> FFACheckBox;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> OnlineCheckBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> OfflineCheckBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> RankedCheckBox;


	virtual bool Initialize() override;

	UFUNCTION() void Onv1CheckBoxStateChanged(bool bIsChecked);
	UFUNCTION() void Onv2CheckBoxStateChanged(bool bIsChecked);
	UFUNCTION() void OnFFACheckBoxStateChanged(bool bIsChecked);

	void SetGameTypeCheckBoxes(TObjectPtr<UCheckBox> EnabledCheckBox,const bool bIsChecked);

	UFUNCTION() void OnOnlineCheckBoxStateChanged(bool bIsChecked);
	UFUNCTION() void OnOfflineCheckBoxStateChanged(bool bIsChecked);
	UFUNCTION() void OnOnlineRankedCheckBoxStateChanged(bool bIsChecked);

	void SetGameNetworkCheckBoxes(TObjectPtr<UCheckBox> EnabledCheckBox, const bool bIsChecked);

	EGameModeType GetGameModeType();
	ENetworkType GetNetworkType();
	EMapId GetMapId();

private:
	UFUNCTION() void OnClickClosePanelBtn();

};
