// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/AnimNotifys/TriggerItemNotify.h"
#include "Characters/Wizard/BaseWizard.h"

void UTriggerItemNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ABaseWizard* Wizard = Cast<ABaseWizard>(OwnerActor))
		{
			if (bItemActivation)
			{
				if (bActivate) Wizard->ActivateItem();
				else Wizard->DeactivateItem();
			}
			else if (bItemChange)
			{
				if (bActivate) Wizard->ChangeItem();
				else Wizard->EndChangeItem();
			}
		}
	}
}
