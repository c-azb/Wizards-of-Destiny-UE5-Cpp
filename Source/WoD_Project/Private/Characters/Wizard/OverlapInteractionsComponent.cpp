// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/OverlapInteractionsComponent.h"
#include "GameTypes/ActorTagsDef.h"
#include "GameTypes/PickUpEnums.h"
#include "Items/Pickups/Pickup.h"
#include "Characters/BaseCharacter.h"

UOverlapInteractionsComponent::UOverlapInteractionsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOverlapInteractionsComponent::SetupOverlapEvent(UPrimitiveComponent* MeshOverlapDetection)
{
	MeshOverlapDetection->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentOverlap);
}

bool UOverlapInteractionsComponent::GetBaseCharacter()
{
	if (BaseCharacter == nullptr)
	{
		BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	}
	return BaseCharacter != nullptr;
}

void UOverlapInteractionsComponent::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(PICKUP_TAG))
	{
		if (APickup* Pickup = Cast<APickup>(OtherActor))
		{
			uint32 PickUpId;
			EPickUpType PickUpType;
			Pickup->Pick(GetOwner(), PickUpId,PickUpType);

			if (GetBaseCharacter())
			{
				BaseCharacter->OnGetPickUp(PickUpType,PickUpId);
			}
			Pickup->Destroy();
		}
	}
}



