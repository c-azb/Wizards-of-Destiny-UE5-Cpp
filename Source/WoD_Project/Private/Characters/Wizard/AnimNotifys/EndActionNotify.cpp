// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/AnimNotifys/EndActionNotify.h"
#include "Characters/Wizard/BaseWizard.h"

void UEndActionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ABaseWizard* BaseWizard = Cast<ABaseWizard>(MeshComp->GetOwner()))
	{
		if (bEndDash)
		{
			BaseWizard->EndDash();
		}
		else if (bEndGetHitHard)
		{
			BaseWizard->EndGetHit();
		}
	}
}
