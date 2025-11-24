// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HoldItem.h"
#include "Components/SkeletalMeshComponent.h"


AHoldItem::AHoldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	TObjectPtr<USceneComponent> HoldPivot = CreateDefaultSubobject<USceneComponent>(FName("HoldPivot"));
	SetRootComponent(HoldPivot);

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ItemSKMesh"));
	ItemMesh->SetupAttachment(GetRootComponent());

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	bReplicates = true;
}

void AHoldItem::AttachHoldItem(TObjectPtr<USkeletalMeshComponent> HoldMesh)
{
	AttachToComponent(HoldMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandSocket"));
}

bool AHoldItem::ActivateItem()
{
	return true;
}

bool AHoldItem::ActivateItemCustom(const float DamageMult, const float DamageRangeMult)
{
	return true;
}


void AHoldItem::DeactivateItem()
{
}

FResourcesStruct AHoldItem::GetItemCost(const bool bEquip)
{
	return FResourcesStruct();
}

bool AHoldItem::GetContinuousItemCost(FResourcesStruct& OutItemCost)
{
	return false;
}

EActivateItem AHoldItem::GetActivateType()
{
	return EActivateItem::EAI_None;
}

EHoldItem AHoldItem::GetItemType()
{
	return EHoldItem::EHI_None;
}

FBoostParameters AHoldItem::GetItemBoost()
{
	return FBoostParameters();
}

bool AHoldItem::IsItemReady()
{
	return true;
}



