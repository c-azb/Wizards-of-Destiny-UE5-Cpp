// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCEnemyAnimInst.generated.h"

class AEnemyNPC;

UCLASS()
class WOD_PROJECT_API UNPCEnemyAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY() AEnemyNPC* EnemyNPC;

	UPROPERTY(BlueprintReadWrite) float MoveSpeed;
	UPROPERTY(BlueprintReadWrite) bool bOnAir;
	UPROPERTY(BlueprintReadWrite) bool bProvoked;
};
