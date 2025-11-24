// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potions/Potion.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APotion::APotion()
{
	StopperMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StopperMesh"));
	StopperMesh->SetupAttachment(ItemMesh);
	StopperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void APotion::BeginPlay()
{
	Super::BeginPlay();
	if (FMath::RandRange(0, 1) > .55f)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickPotionSFX, ItemMesh->GetComponentLocation());
	}
}

FResourcesStruct APotion::GetItemCost(const bool bEquip)
{
	if (bEquip) //free to equip potion, the activation cost is the equipItemCost
	{
		return FResourcesStruct();
	}
	else
	{
		return ActivatePotionCost;
	}
}

EHoldItem APotion::GetItemType()
{
	return EHoldItem::EHI_Potion;
}




