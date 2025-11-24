// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickups/Pickup.h"
#include "Components/SphereComponent.h"
#include "GameTypes/ActorTagsDef.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"	

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupTrigger = CreateDefaultSubobject<USphereComponent>(FName("PickupTrigger"));
	SetRootComponent(PickupTrigger);
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupMesh"));
	PickupMesh->SetupAttachment(GetRootComponent());

	PickupMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Tags.Add(PICKUP_TAG);


	bReplicates = true;
}

void APickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TimerCount == 0)
	{
		RestPosition = GetActorLocation();
		FHitResult HitResult;
		FVector EndRayPos = GetActorLocation() + FVector::DownVector * 100000;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EndRayPos, ECollisionChannel::ECC_WorldStatic))
		{
			RestPosition = HitResult.ImpactPoint + FVector::UpVector * PickupTrigger->GetScaledSphereRadius();
		}
	}
	if (FMath::Abs(GetActorLocation().Z - RestPosition.Z) > 1)
	{
		TimerCount += DeltaSeconds;
		FVector NewLocation = FMath::Lerp<FVector, float>(GetActorLocation(), RestPosition, DeltaSeconds);
		SetActorLocation(NewLocation);
	}
	if (TimerCount >= 1 && PickupTrigger->GetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn) != ECollisionResponse::ECR_Overlap)
	{
		PickupTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}

	AddActorLocalRotation(FQuat(FRotator(0, DeltaSeconds * 40, 0)));
}


void APickup::Pick(AActor* Picker,uint32& OutPickupId, EPickUpType& OutPickupType)
{
	OutPickupId = PickUpID;
	OutPickupType = PickUpType;
	if (Picker)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(OnPickVFX, Picker->GetRootComponent(), FName(), Picker->GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
	}
	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnPickVFX, GetActorLocation());
	}	
}



