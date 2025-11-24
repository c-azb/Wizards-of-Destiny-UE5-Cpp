// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTypes/UIInventoryEnums.h"
#include "InventoryItemWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClickWidget, uint32, ItemId_, EInventoryItemState, CurrItemState);

UCLASS()
class WOD_PROJECT_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> ItemImg;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ItemAmount;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UWidget> ItemAmountOverlay;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UWidget> ItemNameOverlay;

	uint32 ItemId = 0;
	EInventoryItemState ItemState = EInventoryItemState::EIIS_Locked;

	FOnClickWidget OnClickWidget;

	void SetItemState(EInventoryItemState ItemState_);
	void OnClickWidgetBroadcast();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
