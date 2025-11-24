// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuPlayerController.h"
#include "UI/MainMenuUserWidget.h"
#include "UI/StartScreenWidget.h"
#include "GameManagers/WoDGameInstance.h"

#include "Components/Button.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//FInputModeUIOnly InputModeUI;
	//SetInputMode(InputModeUI);
	//SetShowMouseCursor(false);
	bShowMouseCursor = true;

	if (UWoDGameInstance* WoDGameInstance = GetGameInstance<UWoDGameInstance>())
	{
		if (!WoDGameInstance->IsLoggedIn())
		{
			WoDGameInstance->OnLoginEvent.AddDynamic(this, &ThisClass::OnCompleteLogin);
			WoDGameInstance->OnLoadPlayerDataEvent.AddDynamic(this, &ThisClass::OnCompleteLoadData);

			StartScreenWidget = Cast<UStartScreenWidget>(AddWidget(StartScreenWidgetClass));
			WoDGameInstance->StartLogin();
		}
		else
		{
			MainMenuWidget = Cast<UMainMenuUserWidget>(AddWidget(MainMenuWidgetClass));
		}
	}
}

TObjectPtr<UUserWidget> AMainMenuPlayerController::AddWidget(UClass* WidgetClass)
{
	TObjectPtr<UUserWidget> WidgetInst = nullptr;
	if (WidgetClass)
	{
		WidgetInst = CreateWidget(this, WidgetClass);
		if (WidgetInst)
		{
			WidgetInst->AddToViewport();
		}
	}
	return WidgetInst;
}

void AMainMenuPlayerController::OnCompleteLogin(const bool bSuccess)
{
	StartScreenWidget->LoginBtn->OnClicked.Clear();

	if (!bSuccess)
	{
		UWoDGameInstance* WoDGameInstance = GetGameInstance<UWoDGameInstance>();
		StartScreenWidget->LoginBtn->OnClicked.AddDynamic(WoDGameInstance, &UWoDGameInstance::StartLogin);
	}
}

void AMainMenuPlayerController::OnCompleteLoadData(const bool bSuccess)
{
	if (bSuccess)
	{
		MainMenuWidget = Cast<UMainMenuUserWidget>(AddWidget(MainMenuWidgetClass));
		StartScreenWidget->RemoveFromParent();
	}
}
