// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmPanelWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class WOD_PROJECT_API UConfirmPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ConfirmBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> CancelBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ConfirmMessage;

	virtual bool Initialize() override;
	void SetConfirmMessage(const FString& Message);

private:
	UFUNCTION() void OnClickCancelBtn();

};
