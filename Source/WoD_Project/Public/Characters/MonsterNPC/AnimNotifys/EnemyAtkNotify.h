// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyAtkNotify.generated.h"


UCLASS()
class WOD_PROJECT_API UEnemyAtkNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) bool bTriggerAtk = true;
	UPROPERTY(EditAnywhere) float DamageMult = 1;
	UPROPERTY(EditAnywhere) float DamageRangeMult = 1;

	UPROPERTY(EditAnywhere) bool bPreparation = false;
	UPROPERTY(EditAnywhere) bool bJumpAtk = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
