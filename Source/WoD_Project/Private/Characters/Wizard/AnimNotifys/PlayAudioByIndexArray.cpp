// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/AnimNotifys/PlayAudioByIndexArray.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


void UPlayAudioByIndexArray::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp,Animation,EventReference);

	if (PlaySound)
	{
		UAudioComponent* AudioComponent = 
			UGameplayStatics::SpawnSoundAttached(
				PlaySound, 
				MeshComp, 
				FName(), 
				FVector::ZeroVector, 
				EAttachLocation::SnapToTarget, true,0);

		if (AudioComponent)
		{
			AudioComponent->VolumeMultiplier = VolumeMult;
			AudioComponent->SetIntParameter(FName("ClipIndex"), ClipIndex);
			AudioComponent->Play();
		}
	}

}
