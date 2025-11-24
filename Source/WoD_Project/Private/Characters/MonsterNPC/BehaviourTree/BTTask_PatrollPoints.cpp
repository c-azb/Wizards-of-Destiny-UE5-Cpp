// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/BehaviourTree/BTTask_PatrollPoints.h"
#include "AIController.h"
#include "BehaviorTree\BlackboardComponent.h"

UBTTask_PatrollPoints::UBTTask_PatrollPoints()
{
	NodeName = TEXT("UBTTask_PatrollPoints");
}

EBTNodeResult::Type UBTTask_PatrollPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController) return EBTNodeResult::Failed;

	float RandomProb = FMath::RandRange(0.f, 1.f);
	float EndPatrollWaitTime = FMath::RandRange(PatrollTimeRange.X, PatrollTimeRange.Y);

	if (RandomProb < IdleProbability)
	{
		EndPatrollWaitTime *= IdleTimeMultiplier;
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("WaitPatrollTime"), EndPatrollWaitTime);
		return EBTNodeResult::Succeeded;
	}

	if (APawn* OwnerPaw = AIController->GetPawn())
	{
		float Distance = FMath::RandRange(DistanceRange.X, DistanceRange.Y);
		float Angle = FMath::RandRange(-45, 45);
		FVector Direction = OwnerPaw->GetActorForwardVector();

		if (RandomProb > ForwardProbability) Direction *= -1;
		Direction = Direction.RotateAngleAxis(Angle, FVector::UpVector);

		FVector NewPatrollPoint = OwnerPaw->GetActorLocation() + Direction * Distance;
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewPatrollPoint);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("WaitPatrollTime"), EndPatrollWaitTime);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
