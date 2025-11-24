// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameTypes/CharacterAnimEnums.h"

#include "WizardAnimInstance.generated.h"


class ABaseWizard;

UCLASS()
class WOD_PROJECT_API UWizardAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

protected:
	UPROPERTY(BlueprintReadOnly) TObjectPtr<ABaseWizard> Wizard;
	UPROPERTY(BlueprintReadWrite) float VelocityMagSmooth;
	UPROPERTY(BlueprintReadWrite) float MovingAngle;
	UPROPERTY(BlueprintReadWrite) bool IsFalling;
	UPROPERTY(BlueprintReadWrite) bool IsInCombat;
	UPROPERTY(BlueprintReadWrite) bool IsFlying;
	UPROPERTY(BlueprintReadWrite) bool bBlendLocomotionWithSlot=false;
	UPROPERTY(BlueprintReadWrite) float TurnInPlaceAngle;
	UPROPERTY(BlueprintReadWrite) EHandHoldId HandHoldId;
	
	float LastTurnInPlaceAngle=0;




};
