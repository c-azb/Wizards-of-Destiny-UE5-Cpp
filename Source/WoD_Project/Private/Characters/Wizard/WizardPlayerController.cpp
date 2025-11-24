// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/WizardPlayerController.h"
#include "UI/GamePlay/GameplayWidget.h"
#include "UI/BagUserWidget.h"
#include "GameTypes/ResourcesStruct.h"
#include "GameManagers/WoDGameInstance.h"

#include "GameFramework/PlayerInput.h"

void AWizardPlayerController::SetResourcesBars(const FResourcesStruct& ResourcesAmount,const float Health, const FResourcesStruct& MaxResourcesAmount, const float MaxHealth)
{
	if (InicializeGameplayWidget())
	{
		GameplayWidget->SetResourcesBars(ResourcesAmount,MaxResourcesAmount, Health,MaxHealth);
	} 
}

void AWizardPlayerController::OnUpdateScore(const uint32 WizardLvl, const float WizardScorePercent)
{
	if (InicializeGameplayWidget())
	{
		GameplayWidget->SetWizardLevelBars(WizardLvl, WizardScorePercent);
	}
}

bool AWizardPlayerController::InputKey(const FInputKeyParams& Params)
{
	if (!Super::InputKey(Params)) return false;
	
	//needs to add an input action with B on the IMC (no code binding needed)
	if (Params.Key == EKeys::B && Params.Event == EInputEvent::IE_Pressed)
	{
		OpenBagInput();
	}

	return true;
}

void AWizardPlayerController::OpenBagInput()
{
	if (BagWidget == nullptr)
	{
		BagWidget = Cast<UBagUserWidget>(AddWidget(BagWidgetClass));
		
		FInputModeGameAndUI InputModeUI;
		SetInputMode(InputModeUI);
		SetShowMouseCursor(true);
	}
	else
	{
		CloseBag();
	}
}

void AWizardPlayerController::CloseBag()
{
	if (BagWidget)
	{
		FInputModeGameOnly InputModeGame;
		SetInputMode(InputModeGame);
		SetShowMouseCursor(false);

		BagWidget->RemoveFromParent();
		BagWidget = nullptr;
	}
}

bool AWizardPlayerController::InicializeGameplayWidget()
{
	if (!GameplayWidget && IsLocalController())
	{
		GameplayWidget = Cast<UGameplayWidget>(AddWidget(GameplayWidgetClass));
	}
	return GameplayWidget != nullptr;
}

TObjectPtr<UUserWidget> AWizardPlayerController::AddWidget(UClass* WidgetClass)
{
	if (!WidgetClass) return nullptr;

	TObjectPtr<UUserWidget> WidgetInst = nullptr;
	if (WidgetInst = CreateWidget(this, WidgetClass))
	{
		WidgetInst->AddToViewport();
	}
	return WidgetInst;
}

//call on rep controller to send rpc to server to instantiate wizards...
void AWizardPlayerController::GetWizardCharacterID()
{
	if (UWoDGameInstance* WoDGameInstance = GetGameInstance<UWoDGameInstance>())
	{
		uint32 CharacterId, CharacterLvl;
		if (WoDGameInstance->GetEquipedCharacter(CharacterId, CharacterLvl))
		{
			//send rpc to setup character
		}
	}
}
