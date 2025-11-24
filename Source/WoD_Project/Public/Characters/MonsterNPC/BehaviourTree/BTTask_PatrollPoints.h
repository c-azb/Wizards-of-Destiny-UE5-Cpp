// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrollPoints.generated.h"



UCLASS()
class WOD_PROJECT_API UBTTask_PatrollPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_PatrollPoints();
	UPROPERTY(EditAnywhere) float IdleProbability = .3f;
	UPROPERTY(EditAnywhere) float ForwardProbability = .75f;
	UPROPERTY(EditAnywhere) FVector2D PatrollTimeRange = FVector2D(.5f, 2);
	UPROPERTY(EditAnywhere) float IdleTimeMultiplier = 2;
	UPROPERTY(EditAnywhere) FVector2D DistanceRange = FVector2D(300, 1000);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
