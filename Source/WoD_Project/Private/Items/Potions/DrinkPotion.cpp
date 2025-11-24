// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potions/DrinkPotion.h"

FBoostParameters ADrinkPotion::GetItemBoost()
{
	return BoostParameters;
}

bool ADrinkPotion::ActivateItem()
{
	TObjectPtr<USkeletalMeshComponent> ParentSkeleton = GetAttachParentActor()->FindComponentByClass<USkeletalMeshComponent>();
	if (ParentSkeleton)
	{
		StopperMesh->AttachToComponent(ParentSkeleton, FAttachmentTransformRules::KeepWorldTransform, FName("LeftHandSocket"));
	}
	return true;
}

EActivateItem ADrinkPotion::GetActivateType()
{
	return EActivateItem::EAI_DrinkPotion;
}
