// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartScreenWidget.generated.h"

class UButton;

UCLASS()
class WOD_PROJECT_API UStartScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> LoginBtn;
};
