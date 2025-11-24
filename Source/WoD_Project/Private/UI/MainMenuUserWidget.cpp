// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuUserWidget.h"
#include "UI/SubWidget/PlayPanelWidget.h"
#include "UI/SubWidget/ConfirmPanelWidget.h"

#include "Components/Button.h"
#include "Components/PanelWidget.h"

bool UMainMenuUserWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!WizardDuelBtn || !WizardWarBtn || !ChalangesBtn) return true;

	WizardDuelBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickWizardDuel);
	WizardWarBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickWizardWar);
	ChalangesBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickChalanges);
	
	return true;
}

void UMainMenuUserWidget::OnClickWizardDuel()
{
	PlayOption = EPlayOption::EPO_WizardDuel;

	WBPPlayPanel->SetVisibility(ESlateVisibility::Visible);
	//WBPPlayPanel->PlayModesBox->SetVisibility(ESlateVisibility::Visible);
	WBPPlayPanel->PlayBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickPlay);
}

void UMainMenuUserWidget::OnClickWizardWar()
{
	PlayOption = EPlayOption::EPO_WizardWar;

	WBPPlayPanel->SetVisibility(ESlateVisibility::Visible);
	//WBPPlayPanel->PlayModesBox->SetVisibility(ESlateVisibility::Visible);
	WBPPlayPanel->PlayBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickPlay);
}

void UMainMenuUserWidget::OnClickChalanges()
{
	PlayOption = EPlayOption::EPO_Chalange;

	WBPConfirmPanel->ConfirmBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickConfirmBtn);
	WBPConfirmPanel->SetConfirmMessage(FString("You are entering the Wizard Chalanges!"));
	WBPConfirmPanel->SetVisibility(ESlateVisibility::Visible);
	//setup action and bind buttons
}

void UMainMenuUserWidget::OnClickPlay()
{
	WBPPlayPanel->PlayBtn->OnClicked.Clear();
	WBPConfirmPanel->ConfirmBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickConfirmBtn);

	FString ConfirmMessage = "Let's Duel!";
		//PlayOption == EPlayOption::EPO_Ranked ? "Let's Duel!\nRanked Mode" : 
		//"Let's Duel!\nQuick Duel Mode";
		
	WBPConfirmPanel->SetConfirmMessage(ConfirmMessage);
	WBPConfirmPanel->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuUserWidget::OnClickConfirmBtn()
{
	WBPConfirmPanel->ConfirmBtn->OnClicked.Clear();

	EGameModeType GameModeType = EGameModeType::EGMT_None;
	ENetworkType NetworkType = ENetworkType::ENT_None;
	EMapId MapId = WBPPlayPanel->GetMapId();

	if (PlayOption == EPlayOption::EPO_WizardDuel)
	{
		GameModeType = WBPPlayPanel->GetGameModeType();
		NetworkType = WBPPlayPanel->GetNetworkType();
	}
	else if (PlayOption == EPlayOption::EPO_WizardWar)
	{
		GameModeType = EGameModeType::EGMT_WizardWar;
		NetworkType = WBPPlayPanel->GetNetworkType();
	}
	else if (PlayOption == EPlayOption::EPO_Chalange)
	{
		GameModeType = EGameModeType::EGMT_Chalange;
		NetworkType = ENetworkType::ENT_Offline;
	}

	//process game mode type / network type -> search match or load map, .....
}

