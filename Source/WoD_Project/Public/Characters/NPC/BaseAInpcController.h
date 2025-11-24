// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameTypes/EnemyEnums.h"
#include "BaseAInpcController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

UCLASS()
class WOD_PROJECT_API ABaseAInpcController : public AAIController
{
	GENERATED_BODY()

public:
	AActor* CurrentTarget = nullptr;

	ABaseAInpcController();
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	bool CheckCurrentTarget();
	ETargetType GetTargetType(const AActor* Target_);

	UFUNCTION() virtual void DecisionsPicker();

protected:
	UPROPERTY(EditAnywhere, Category = "Att_AIEnemyCtrlr") TObjectPtr<UBehaviorTree> BehaviourTree;

	UPROPERTY(VisibleAnywhere) TObjectPtr<UAIPerceptionComponent> AIPerception;
	TObjectPtr<UAISenseConfig_Sight> AISenseConfigSight = nullptr;
	TObjectPtr<UAISenseConfig_Damage> AISenseConfigDamage = nullptr;

	virtual void OnPossess(APawn* InPawn);

	UFUNCTION() virtual void OnPerceptionTargetUpdated(AActor* Actor, FAIStimulus Stimulus);
	TObjectPtr<AActor> GetCloserPerceivedActor(TArray<AActor*>& PerceivedActors, const bool OnSight = false);
	TObjectPtr<AActor> GetCloserActor(AActor* Actor1, AActor* Actor2);

	bool IsTargetAlive(AActor* Target);
	bool IsActorInSightRadius(AActor* Actor1);
	bool IsTargetReachable(const AActor* Target);
	void SetCurrentTarget(AActor* Target);

	UFUNCTION() virtual void DisableAI();

};
