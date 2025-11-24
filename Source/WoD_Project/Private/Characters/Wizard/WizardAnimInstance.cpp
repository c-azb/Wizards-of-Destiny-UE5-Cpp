// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/WizardAnimInstance.h"
#include "Characters/Wizard/BaseWizard.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameTypes/WizardStates.h"


void UWizardAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Wizard = Cast<ABaseWizard>(TryGetPawnOwner());
}

void UWizardAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Wizard)
	{
		IsFalling = Wizard->GetCharacterMovement()->IsFalling();

		IsFlying = Wizard->WizardState == EWizardState::EWCS_FlyingBroom;
		VelocityMagSmooth = (IsFlying ? Wizard->GetVelocity().Size() : Wizard->GetVelocity().Size2D()) / Wizard->GetMaxSpeed();

		IsInCombat = Wizard->bUseControllerRotationYaw && !IsFlying;

		bBlendLocomotionWithSlot = (VelocityMagSmooth > .05f || IsFalling) && !Wizard->IsDashing && !Wizard->IsGettingHitHard();

		HandHoldId = (EHandHoldId)Wizard->GetCurrentItemHandHoldId();
		//LastTurnInPlaceAngle = FMath::FindDeltaAngleDegrees<float, float>(Wizard->GetActorRotation().Yaw, LastTurnInPlaceAngle);

		if (IsInCombat || IsFlying)
		{
			float WizardRotYaw = Wizard->GetActorRotation().Yaw;
			float VelocityYaw = Wizard->GetVelocity().Rotation().Yaw;
			MovingAngle = FMath::FindDeltaAngleDegrees<float, float>(VelocityYaw, WizardRotYaw);
		}
				
	}

}
