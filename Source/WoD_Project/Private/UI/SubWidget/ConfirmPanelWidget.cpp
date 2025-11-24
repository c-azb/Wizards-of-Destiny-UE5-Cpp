// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/ConfirmPanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UConfirmPanelWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (!CancelBtn) return true;

    CancelBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickCancelBtn);
    SetVisibility(ESlateVisibility::Collapsed);

    return true;
}

void UConfirmPanelWidget::SetConfirmMessage(const FString& Message)
{
    ConfirmMessage->SetText(FText::FromString(Message));
}

void UConfirmPanelWidget::OnClickCancelBtn()
{
    ConfirmBtn->OnClicked.Clear();
    SetVisibility(ESlateVisibility::Collapsed);
}
