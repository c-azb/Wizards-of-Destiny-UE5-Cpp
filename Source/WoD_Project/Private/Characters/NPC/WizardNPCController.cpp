// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/WizardNPCController.h"
#include "Characters/NPC/NPCWizard.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "GameTypes/ActorTagsDef.h"

AWizardNPCController::AWizardNPCController()
{
	bWantsPlayerState = true;
}

void AWizardNPCController::DecisionsPicker()
{
	if (!GetNPCWizard()) return;
	
	if (!NPCWizard->GetCanMove())
	{
		NPCWizard->SetMovementState(EEnemyMovementState::EES_NoMovement);
	}
	else if (CurrentTarget == nullptr)
	{
		NPCWizard->SetMovementState(EEnemyMovementState::EES_Patrolling);
	}
	else if (CheckCurrentTarget())
	{
		if (LineOfSightTo(CurrentTarget))
		{
			NPCWizard->SetMovementState(EEnemyMovementState::EES_Combat);
		}
		else
		{
			NPCWizard->SetMovementState(EEnemyMovementState::EES_Chasing);
		}
	}

	if (NPCWizard->GetMovementState() == EEnemyMovementState::EES_Combat)
	{
		//pick combat decisions
	}
	

}

void AWizardNPCController::SetBTWizardMovementState(const EEnemyMovementState MovementState_)
{
	GetBlackboardComponent()->SetValueAsEnum(FName("WizardMovementState"), (uint8)MovementState_);
}

bool AWizardNPCController::GetTargetAim(FRotator& OutControllerRot)
{
	if (CurrentTarget == nullptr) return false;

	FVector ViewLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector EndPositions[3];

	for (int i = 0; i < 3; i++)
	{
		EndPositions[i] = CurrentTarget->GetActorLocation() + FVector::UpVector * i * 10;
		FHitResult HitResult;

		FCollisionQueryParams CollisionQuerry;
		CollisionQuerry.AddIgnoredActor(GetPawn());

		GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			ViewLocation, 
			EndPositions[i], 
			ECollisionChannel::ECC_Visibility, 
			CollisionQuerry);

		if (AActor* HitActor = HitResult.GetActor())
		{
			if (GetTargetType(HitActor) != ETargetType::ETT_None)
			{
				OutControllerRot = (EndPositions[i] - ViewLocation).Rotation();
				return true;
			}
		}
	}

	return false;

}

bool AWizardNPCController::GetNPCWizard()
{
	if (NPCWizard == nullptr && GetPawn())
	{
		NPCWizard = Cast<ANPCWizard>(GetPawn());
	}
	return NPCWizard!=nullptr;
}
