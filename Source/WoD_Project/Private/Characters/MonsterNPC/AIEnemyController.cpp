// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/AIEnemyController.h"
#include "Characters/MonsterNPC/EnemyNPC.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "GameTypes/ActorTagsDef.h"


void AAIEnemyController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);
	//updates movement orientation to follow controller rot or not when very small velocity and chasing
	if (GetEnemyNPC() && EnemyNPC->EnemyState == EEnemyState::EES_Chasing)
	{   
		EnemyNPC->SetMovementState();
	}
}

void AAIEnemyController::DecisionsPicker()
{
	if (!IsAlive())
	{
		SetEnemyState(EEnemyState::EES_Death);
		return;
	}
	else if (IsGettingHitHard())
	{
		return;
	}

	if (CurrentTarget)
	{
		if (GetActorsInAtkRange(EEnemyState::EES_CloseAttack))
		{
			SetEnemyState(EEnemyState::EES_CloseAttack);
		}
		else if (GetActorsInAtkRange(EEnemyState::EES_FarAttack))
		{
			SetEnemyState(EEnemyState::EES_FarAttack);
		}
		else if (CheckCurrentTarget())
		{
			SetEnemyState(EEnemyState::EES_Chasing);
		}
		else
		{
			SetEnemyState(EEnemyState::EES_Patrolling);
		}
	}
	else
	{
		SetEnemyState(EEnemyState::EES_Patrolling);
	}
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (HasAuthority())
	{
		SetEnemyState(EEnemyState::EES_Patrolling);
	}
}

void AAIEnemyController::SetEnemyState(const EEnemyState NewEnemyState)
{
	if (GetEnemyNPC() && EnemyNPC->SetEnemyState(NewEnemyState))
	{
		GetBlackboardComponent()->SetValueAsEnum(FName("EnemyState"), (uint8)NewEnemyState);
	}
}

bool AAIEnemyController::GetActorsInAtkRange(const EEnemyState AttackStateType)
{
	if (GetEnemyNPC())
	{
		if (EnemyNPC->EnemyState >= AttackStateType)
		{
			return EnemyNPC->EnemyState == AttackStateType;
		}

		TArray<FOverlapResult> OverlappedActors;
		
		if (EnemyNPC->GetActorsInAtkRange(OverlappedActors,AttackStateType))
		{
			TArray<AActor*> WizardsOverlapped;
			for (const FOverlapResult& OverlappedActor : OverlappedActors)
			{
				if (OverlappedActor.GetActor()->ActorHasTag(WIZARD_TAG))
				{
					WizardsOverlapped.Add(OverlappedActor.GetActor());
				}
			}
			if (WizardsOverlapped.Num() > 0)
			{
				SetCurrentTarget(GetCloserPerceivedActor(WizardsOverlapped,true));
				return CurrentTarget != nullptr;
			}
		}
	}
	return false;
}

float AAIEnemyController::GetFarAttackRange()
{
	if (GetEnemyNPC())
	{
		return EnemyNPC->FarAttackRange;
	}
	return -1.f;
}

void AAIEnemyController::DisableAI()
{
	Super::DisableAI();
	SetEnemyState(EEnemyState::EES_Death);
}

bool AAIEnemyController::IsAlive()
{
	if (GetEnemyNPC())
	{
		return EnemyNPC->IsAlive();
	}
	return false;
}

bool AAIEnemyController::IsGettingHitHard()
{
	if (GetEnemyNPC())
	{
		return EnemyNPC->IsGettingHitHard();
	}
	return false;
}


bool AAIEnemyController::GetEnemyNPC()
{
	if (!EnemyNPC)
	{
		EnemyNPC = Cast<AEnemyNPC>(GetPawn());
	}
	return EnemyNPC != nullptr;
}


