// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayAudioByIndexArray.generated.h"


UCLASS()
class WOD_PROJECT_API UPlayAudioByIndexArray : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere) uint32 ClipIndex;
	UPROPERTY(EditAnywhere) float VolumeMult=1;
	UPROPERTY(EditAnywhere, Category = "Att_Spell") TObjectPtr<USoundBase> PlaySound;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
