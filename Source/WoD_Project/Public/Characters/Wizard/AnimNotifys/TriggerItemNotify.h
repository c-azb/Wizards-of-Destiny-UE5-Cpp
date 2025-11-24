// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TriggerItemNotify.generated.h"

/**
 * 
 */
UCLASS()
class WOD_PROJECT_API UTriggerItemNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	UPROPERTY(EditAnywhere) bool bItemActivation= false;
	UPROPERTY(EditAnywhere) bool bItemChange = false;
	UPROPERTY(EditAnywhere) bool bActivate = false;
	
};
