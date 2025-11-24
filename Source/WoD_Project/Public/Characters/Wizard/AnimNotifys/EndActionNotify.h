// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndActionNotify.generated.h"

UCLASS()
class WOD_PROJECT_API UEndActionNotify : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere) bool bEndDash = false;
	UPROPERTY(EditAnywhere) bool bEndGetHitHard = false;

	
};
