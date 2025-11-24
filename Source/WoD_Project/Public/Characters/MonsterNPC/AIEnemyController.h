// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/BaseAInpcController.h"
#include "GameTypes/EnemyEnums.h"
#include "AIEnemyController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class AEnemyNPC;

UCLASS()
class WOD_PROJECT_API AAIEnemyController : public ABaseAInpcController
{
	GENERATED_BODY()

public:
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	virtual void DecisionsPicker() override;
	void SetEnemyState(const EEnemyState NewEnemyState);
	bool GetActorsInAtkRange(const EEnemyState AttackStateType);
	float GetFarAttackRange();
	virtual void DisableAI() override;
	bool IsAlive();
	bool IsGettingHitHard();

protected:
	virtual void OnPossess(APawn* InPawn);

private:
	bool GetEnemyNPC();
	AEnemyNPC* EnemyNPC;
};
