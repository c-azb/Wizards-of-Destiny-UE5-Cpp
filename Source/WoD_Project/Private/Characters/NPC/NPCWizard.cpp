// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/NPCWizard.h"
#include "Characters/NPC/WizardNPCController.h"



void ANPCWizard::InitializeCharacter()
{
	Super::InitializeCharacter();
	if (HasAuthority())
	{
		InitializeItems(WizardMWCode);
	}
}

void ANPCWizard::SetMovementState(const EEnemyMovementState MovementState_)
{
	if (MovementState_ != MovementState)
	{
		MovementState = MovementState_;

		if (GetNPCController())
		{
			WizarndNPCController->SetBTWizardMovementState(MovementState_);
		}
	}
}

bool ANPCWizard::GetNPCController()
{
	if (WizarndNPCController == nullptr)
	{
		WizarndNPCController = Cast<AWizardNPCController>(GetController());
	}
	return WizarndNPCController != nullptr;
}

bool ANPCWizard::GetCanMove()
{
	return CanMove();
}
