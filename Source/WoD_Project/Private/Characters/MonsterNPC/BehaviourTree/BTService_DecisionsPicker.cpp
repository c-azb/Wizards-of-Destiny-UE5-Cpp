// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/BehaviourTree/BTService_DecisionsPicker.h"
#include "Characters/NPC/BaseAInpcController.h"
#include "GameTypes/EnemyEnums.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_EnemyStateSet::UBTService_EnemyStateSet()
{
	NodeName = TEXT("DecisionsPicker");
}

void UBTService_EnemyStateSet::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AIController->HasAuthority())
		{
			if (ABaseAInpcController* BaseAInpcController = Cast<ABaseAInpcController>(AIController))
			{
				BaseAInpcController->DecisionsPicker();
			}
		}
	}
}
