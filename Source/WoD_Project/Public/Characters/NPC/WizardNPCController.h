// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/BaseAInpcController.h"
#include "GameTypes/WizardStates.h"
#include "GameTypes/EnemyEnums.h"
#include "WizardNPCController.generated.h"

class ANPCWizard;

UCLASS()
class WOD_PROJECT_API AWizardNPCController : public ABaseAInpcController
{
	GENERATED_BODY()

public:
	AWizardNPCController();

	virtual void DecisionsPicker() override;
	void SetBTWizardMovementState(const EEnemyMovementState MovementState_);

private:
	ANPCWizard* NPCWizard = nullptr;

	bool GetTargetAim(FRotator& OutControllerRot);

	bool GetNPCWizard();
	
};
