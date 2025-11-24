// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Wizard/BaseWizard.h"
#include "GameTypes/EnemyEnums.h"
#include "NPCWizard.generated.h"

class AWizardNPCController;

UCLASS()
class WOD_PROJECT_API ANPCWizard : public ABaseWizard
{
	GENERATED_BODY()


protected:
	virtual void InitializeCharacter() override;
private:

	UPROPERTY(EditAnywhere, Category = "Att_NPCWizard") uint32 WizardMWCode;

	EEnemyMovementState MovementState = EEnemyMovementState::EES_Patrolling;

	AWizardNPCController* WizarndNPCController = nullptr;

	bool GetNPCController();

public:
	void SetMovementState(const EEnemyMovementState MovementState_);
	FORCEINLINE EEnemyMovementState GetMovementState() const { return MovementState; };
	bool GetCanMove();

};
