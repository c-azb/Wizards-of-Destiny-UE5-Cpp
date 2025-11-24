// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MagicWand.h"
#include "Items/Spells/Spell.h"

AMagicWand::AMagicWand()
{
	WandPoint = CreateDefaultSubobject<USceneComponent>(FName("WandPoint"));
	WandPoint->SetupAttachment(GetRootComponent());
}

bool AMagicWand::ActivateItem()
{
	if (PendingSpell == nullptr) return false;

	FRotator AimRotation;
	GetAimTargetPos(PendingSpell->MaxSpellDistance, PendingSpell->MaxSpellDistance,nullptr, &AimRotation, nullptr);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();
	CurrentSpellInst = GetWorld()->SpawnActor<ASpell>(PendingSpell->GetClass(), WandPoint->GetComponentLocation(), AimRotation, SpawnParameters);
	PendingSpell = nullptr;

	CurrentSpellInst = CurrentSpellInst->ActivateItemType == EActivateItem::EAI_ThrowSpell ? nullptr : CurrentSpellInst;
	return true;	
}

void AMagicWand::DeactivateItem()
{
	if (GetActivateType() == EActivateItem::EAI_HoldSpell && CurrentSpellInst)
	{
		CurrentSpellInst->Destroy();
	}
	
	CurrentSpellInst = nullptr;
	PendingSpell = nullptr;
}

FResourcesStruct AMagicWand::GetItemCost(const bool bEquip)
{
	if (bEquip)
	{
		return EquipItemCost;
	}
	else if (PendingSpell)
	{
		return PendingSpell->SpellCosts;
	}
	else if (CurrentSpellInst)
	{
		return CurrentSpellInst->SpellCosts;
	}
	return FResourcesStruct();
}

bool AMagicWand::GetContinuousItemCost(FResourcesStruct& OutItemCost)
{
	if (CurrentSpellInst && CurrentSpellInst->ActivateItemType == EActivateItem::EAI_HoldSpell)
	{
		OutItemCost = CurrentSpellInst->SpellCosts;
		return true;
	}
	return false;
}


EActivateItem AMagicWand::GetActivateType()
{
	if (PendingSpell)
	{
		return PendingSpell->ActivateItemType;
	}
	else if (CurrentSpellInst)
	{
		return CurrentSpellInst->ActivateItemType;
	}
	return EActivateItem::EAI_None;
}

EHoldItem AMagicWand::GetItemType()
{
	return EHoldItem::EHI_MagicWand;
}

bool AMagicWand::GetAimTargetPos(const float MaxDistance,const float CurrentReach, FHitResult* OutHitResult, FRotator* OutRot, FVector* OutHitPoint)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	GetInstigatorController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FHitResult HitResult;

	FVector ViewDirection = ViewRotation.Vector();
	FVector MWDirection = ViewDirection;
	FVector StartPoint = ViewLocation + ViewDirection * FVector::Dist2D(GetOwner()->GetActorLocation(), ViewLocation);
	FVector EndPoint = StartPoint + ViewDirection * (MaxDistance + 100); //could offset to infinity, but uses maxdistance+offset 
																			// to reduce useless collisions

	FCollisionQueryParams CollisionQuery;
	CollisionQuery.AddIgnoredActor(GetOwner());
	CollisionQuery.AddIgnoredActor(this);
	if (CurrentSpellInst) CollisionQuery.AddIgnoredActor(CurrentSpellInst);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECollisionChannel::ECC_Visibility,
		CollisionQuery
	);

	StartPoint = WandPoint->GetComponentLocation();
	MWDirection = bHit ? (HitResult.ImpactPoint - StartPoint).GetSafeNormal() :
					   (EndPoint - StartPoint).GetSafeNormal();
	EndPoint = StartPoint + MWDirection * CurrentReach;

	bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECollisionChannel::ECC_Visibility,
		CollisionQuery
	);

	if (bHit)
	{
		EndPoint = HitResult.ImpactPoint;
		MWDirection = (HitResult.ImpactPoint - StartPoint).GetSafeNormal();
	}

	if (OutRot) *OutRot = MWDirection.Rotation();
	if (OutHitPoint) *OutHitPoint = EndPoint;
	if (OutHitResult) *OutHitResult = HitResult;
	return bHit;
}

bool AMagicWand::IsItemReady()
{
	return PendingSpell != nullptr;
}


bool AMagicWand::SetupSpell(UClass* SpellClass, const uint32 CurrentWizardLvl)
{
	if (PendingSpell = Cast<ASpell>(SpellClass->GetDefaultObject()))
	{
		if (PendingSpell->MinWizardLvl <= CurrentWizardLvl)
		{
			return true;
		}
	}
	PendingSpell = nullptr;
	return false;
}


