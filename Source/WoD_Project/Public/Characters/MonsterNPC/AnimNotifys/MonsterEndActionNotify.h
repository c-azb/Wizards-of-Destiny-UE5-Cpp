// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MonsterEndActionNotify.generated.h"

/**
 * 
 */
UCLASS()
class WOD_PROJECT_API UMonsterEndActionNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere) bool bEndHitHard=false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
