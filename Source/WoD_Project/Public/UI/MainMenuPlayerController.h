// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UMainMenuUserWidget;
class UStartScreenWidget;

UCLASS()
class WOD_PROJECT_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Att_HUD") TSubclassOf<UMainMenuUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Att_HUD") TSubclassOf<UStartScreenWidget> StartScreenWidgetClass;
	UPROPERTY() TObjectPtr<UStartScreenWidget> StartScreenWidget;
	UPROPERTY() TObjectPtr<UMainMenuUserWidget> MainMenuWidget;

	TObjectPtr<UUserWidget> AddWidget(UClass* WidgetClass);
	UFUNCTION() void OnCompleteLogin(const bool bSuccess);
	UFUNCTION() void OnCompleteLoadData(const bool bSuccess);
};
