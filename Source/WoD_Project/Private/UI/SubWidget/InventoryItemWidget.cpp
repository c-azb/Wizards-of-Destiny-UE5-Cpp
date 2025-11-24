// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/InventoryItemWidget.h"

void UInventoryItemWidget::SetItemState(EInventoryItemState ItemState_)
{
	SetVisibility(ItemState_ == EInventoryItemState::EIIS_Empty ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	ItemAmountOverlay->SetVisibility(ItemState_ == EInventoryItemState::EIIS_Occupied ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ItemNameOverlay->SetVisibility(ItemState_ == EInventoryItemState::EIIS_Occupied ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ItemState = ItemState_;
	OnClickWidget.Clear();
	//FOnPointerEvent
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (ItemState != EInventoryItemState::EIIS_Locked)
	{
		OnClickWidget.Broadcast(ItemId,ItemState);
	}

	return Reply;
}
