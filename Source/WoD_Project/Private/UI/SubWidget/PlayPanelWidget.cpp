// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubWidget/PlayPanelWidget.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"

bool UPlayPanelWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!v1CheckBox || !v2CheckBox || !FFACheckBox || !ClosePlayPanelBtn || !OfflineCheckBox || !OnlineCheckBox || !RankedCheckBox) return false;

	ClosePlayPanelBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickClosePanelBtn);

	v1CheckBox->SetIsChecked(true);
	v2CheckBox->SetIsChecked(false);
	FFACheckBox->SetIsChecked(false);

	v1CheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::Onv1CheckBoxStateChanged);
	v2CheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::Onv2CheckBoxStateChanged);
	FFACheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnFFACheckBoxStateChanged);
	
	OfflineCheckBox->SetIsChecked(true);
	OnlineCheckBox->SetIsChecked(false);
	RankedCheckBox->SetIsChecked(false);

	OfflineCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnOfflineCheckBoxStateChanged);
	OnlineCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnOnlineCheckBoxStateChanged);
	RankedCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnOnlineRankedCheckBoxStateChanged);

	SetVisibility(ESlateVisibility::Collapsed);

	return true;
}

void UPlayPanelWidget::Onv1CheckBoxStateChanged(bool bIsChecked)
{
	SetGameTypeCheckBoxes(v1CheckBox, bIsChecked);
}

void UPlayPanelWidget::Onv2CheckBoxStateChanged(bool bIsChecked)
{
	SetGameTypeCheckBoxes(v2CheckBox, bIsChecked);
}

void UPlayPanelWidget::OnFFACheckBoxStateChanged(bool bIsChecked)
{
	SetGameTypeCheckBoxes(FFACheckBox, bIsChecked);
}

void UPlayPanelWidget::SetGameTypeCheckBoxes(TObjectPtr<UCheckBox> EnabledCheckBox, const bool bIsChecked)
{
	if (bIsChecked)
	{
		v1CheckBox->SetIsEnabled(v1CheckBox != EnabledCheckBox);
		v2CheckBox->SetIsEnabled(v2CheckBox != EnabledCheckBox);
		FFACheckBox->SetIsEnabled(FFACheckBox != EnabledCheckBox);

		v1CheckBox->SetIsChecked(v1CheckBox == EnabledCheckBox);
		v2CheckBox->SetIsChecked(v2CheckBox == EnabledCheckBox);
		FFACheckBox->SetIsChecked(FFACheckBox == EnabledCheckBox);
	}
}

void UPlayPanelWidget::OnOnlineCheckBoxStateChanged(bool bIsChecked)
{
	SetGameNetworkCheckBoxes(OnlineCheckBox, bIsChecked);
}

void UPlayPanelWidget::OnOfflineCheckBoxStateChanged(bool bIsChecked)
{
	SetGameNetworkCheckBoxes(OfflineCheckBox, bIsChecked);
}

void UPlayPanelWidget::OnOnlineRankedCheckBoxStateChanged(bool bIsChecked)
{
	SetGameNetworkCheckBoxes(RankedCheckBox, bIsChecked);
}

void UPlayPanelWidget::SetGameNetworkCheckBoxes(TObjectPtr<UCheckBox> EnabledCheckBox, const bool bIsChecked)
{
	if (bIsChecked)
	{
		OfflineCheckBox->SetIsEnabled(OfflineCheckBox != EnabledCheckBox);
		OnlineCheckBox->SetIsEnabled(OnlineCheckBox != EnabledCheckBox);
		RankedCheckBox->SetIsEnabled(OnlineCheckBox != EnabledCheckBox);

		OfflineCheckBox->SetIsChecked(OfflineCheckBox == EnabledCheckBox);
		OnlineCheckBox->SetIsChecked(OnlineCheckBox == EnabledCheckBox);
		RankedCheckBox->SetIsChecked(OnlineCheckBox == EnabledCheckBox);
	}
}

EGameModeType UPlayPanelWidget::GetGameModeType()
{
	if (v1CheckBox->IsChecked())
	{
		return EGameModeType::EGMT_1v1;
	}
	else if (v2CheckBox->IsChecked())
	{
		return EGameModeType::EGMT_2v2;
	}
	else if (FFACheckBox->IsChecked())
	{
		return EGameModeType::EGMT_FFA;
	}

	return EGameModeType::EGMT_None;
}

ENetworkType UPlayPanelWidget::GetNetworkType()
{
	if (OnlineCheckBox->IsChecked())
	{
		return ENetworkType::ENT_Online;
	}
	else if (OfflineCheckBox->IsChecked())
	{
		return ENetworkType::ENT_Offline;
	}
	else if (RankedCheckBox->IsChecked())
	{
		return ENetworkType::ENT_Online_Ranked;
	}
	return ENetworkType::ENT_None;
}

EMapId UPlayPanelWidget::GetMapId()
{
	if (MapPickComboBox->GetSelectedIndex() >= 0 && MapPickComboBox->GetSelectedIndex() < (uint8)EMapId::EMI_None)
	{
		return (EMapId)MapPickComboBox->GetSelectedIndex();
	}
	return EMapId::EMI_None;
}

void UPlayPanelWidget::OnClickClosePanelBtn()
{
	PlayBtn->OnClicked.Clear();
	SetVisibility(ESlateVisibility::Collapsed);
}
