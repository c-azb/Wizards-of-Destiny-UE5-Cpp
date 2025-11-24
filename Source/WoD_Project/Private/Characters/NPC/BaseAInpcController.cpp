// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/BaseAInpcController.h"
#include "Characters/BaseCharacter.h"

#include "BehaviorTree\BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
//#include "Perception/AISenseConfig_Hearing.h"

#include "GameTypes/ActorTagsDef.h"

#include "AI/Navigation/NavigationTypes.h"

#include "NavigationSystem.h"
#include "BrainComponent.h"


ABaseAInpcController::ABaseAInpcController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("NPCAIPerception"));

	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("NPCSightSense"));
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;

	AISenseConfigDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("NPCDamageSense"));

	AIPerception->ConfigureSense(*AISenseConfigSight);
	AIPerception->ConfigureSense(*AISenseConfigDamage);

	AIPerception->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	//AAIController::SetGenericTeamId(FGenericTeamId(1));
	bReplicates = true;
}

void ABaseAInpcController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		FRotator NewControlRotation = GetControlRotation();

		const FVector FocalPoint = CurrentTarget ? CurrentTarget->GetActorLocation() : GetFocalPoint();

		if (FAISystem::IsValidLocation(FocalPoint) && MyPawn->bUseControllerRotationYaw)
		{
			NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation();
			NewControlRotation = FMath::RInterpTo(GetControlRotation(), NewControlRotation, DeltaTime, 10);
		}
		else
		{
			NewControlRotation = MyPawn->GetActorRotation();
		}

		SetControlRotation(NewControlRotation);

		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

		if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
		{
			MyPawn->FaceRotation(NewControlRotation, DeltaTime);
		}
	}
}

bool ABaseAInpcController::CheckCurrentTarget()
{
	if (!IsTargetAlive(CurrentTarget) || !IsActorInSightRadius(CurrentTarget) || !IsTargetReachable(CurrentTarget))
	{
		TArray<AActor*> PerceivedActors;
		AIPerception->GetKnownPerceivedActors(TSubclassOf<UAISense>(), PerceivedActors);
		SetCurrentTarget(GetCloserPerceivedActor(PerceivedActors));
	}

	return CurrentTarget != nullptr;
}

ETargetType ABaseAInpcController::GetTargetType(const AActor* Target_)
{
	if (Target_->ActorHasTag(WIZARD_TAG))
	{
		return ETargetType::ETT_Wizard;
	}
	else if (Target_->ActorHasTag(SPELL_TAG))
	{
		return ETargetType::ETT_Spell;
	}
	else if (Target_->ActorHasTag(MONSTER_TAG))
	{
		return ETargetType::ETT_Monster;
	}
	return ETargetType::ETT_None;
}

void ABaseAInpcController::DecisionsPicker(){}

void ABaseAInpcController::DisableAI()
{
	AIPerception->OnTargetPerceptionUpdated.Clear();
	AIPerception->ForgetAll();
	GetBrainComponent()->Cleanup();
	CurrentTarget = nullptr;
}

void ABaseAInpcController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviourTree && RunBehaviorTree(BehaviourTree) && AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionTargetUpdated);
	}
}

void ABaseAInpcController::OnPerceptionTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	TArray<AActor*> PerceivedActors;
	AIPerception->GetKnownPerceivedActors(TSubclassOf<UAISense>(), PerceivedActors);
	bool OnSight = Stimulus.WasSuccessfullySensed() && Stimulus.Type == 0;
	SetCurrentTarget(GetCloserPerceivedActor(PerceivedActors, OnSight));
}

TObjectPtr<AActor> ABaseAInpcController::GetCloserPerceivedActor(TArray<AActor*>& PerceivedActors, const bool OnSight)
{
	AActor* TargetPick = nullptr;

	for (auto PerceivedActor : PerceivedActors)
	{
		if (!IsTargetAlive(PerceivedActor) || !IsActorInSightRadius(PerceivedActor) || !IsTargetReachable(PerceivedActor))
		{
			continue;
		}
		else if (OnSight && !LineOfSightTo(PerceivedActor))
		{
			continue;
		}
		else if (!TargetPick)
		{
			TargetPick = PerceivedActor;
		}
		else
		{
			TargetPick = GetCloserActor(TargetPick, PerceivedActor);
		}
	}
	return TargetPick;
}

TObjectPtr<AActor> ABaseAInpcController::GetCloserActor(AActor* Actor1, AActor* Actor2)
{
	float Dist1 = (Actor1->GetActorLocation() - GetPawn()->GetActorLocation()).SquaredLength();
	float Dist2 = (Actor2->GetActorLocation() - GetPawn()->GetActorLocation()).SquaredLength();
	return Dist1 >= Dist2 ? Actor1 : Actor2;
}

bool ABaseAInpcController::IsTargetAlive(AActor* Target)
{
	if (Target)
	{
		if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Target))
		{
			if (!BaseCharacter->IsAlive())
			{
				AIPerception->ForgetActor(Target);
			}
			return BaseCharacter->IsAlive();
		}
	}
	return false;
}

bool ABaseAInpcController::IsActorInSightRadius(AActor* Actor1)
{
	if (Actor1)
	{
		float LoseSight = AISenseConfigSight->LoseSightRadius;
		float Dist = (Actor1->GetActorLocation() - GetPawn()->GetActorLocation()).SquaredLength();
		bool IsInRadiusSight = Dist < LoseSight * LoseSight;
		if (!IsInRadiusSight)
		{
			AIPerception->ForgetActor(Actor1);
		}
		return IsInRadiusSight;
	}
	return false;
}

bool ABaseAInpcController::IsTargetReachable(const AActor* Target)
{
	if (!GetPawn() || !Target) return false;

	if (GetPawn()->GetVelocity().SquaredLength() > 50) return true; //don't test path if moving

	if (const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		if (const ANavigationData* NavData = NavSys->GetNavDataForProps(GetNavAgentPropertiesRef()))
		{
			FSharedConstNavQueryFilter QueryFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, TSubclassOf<UNavigationQueryFilter>());
			FPathFindingQuery PathFindingQuerry(this, *NavData, GetPawn()->GetActorLocation(), Target->GetActorLocation(), QueryFilter);
			return NavSys->TestPathSync(PathFindingQuerry);
		}
	}
	return false;
}

void ABaseAInpcController::SetCurrentTarget(AActor* Target)
{
	if (Target && Target != CurrentTarget)
	{
		CurrentTarget = Target;
		GetBlackboardComponent()->SetValueAsObject(FName("CurrentTarget"), CurrentTarget);
	}
	else if (!Target)
	{
		CurrentTarget = nullptr;
		GetBlackboardComponent()->ClearValue(FName("CurrentTarget"));
	}
}
