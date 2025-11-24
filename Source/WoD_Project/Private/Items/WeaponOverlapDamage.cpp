// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WeaponOverlapDamage.h"
#include "Items/ApplyDamageComponent.h"
#include "Components/BoxComponent.h"

AWeaponOverlapDamage::AWeaponOverlapDamage()
{
	ApplyDamageComponent = CreateDefaultSubobject<UApplyDamageComponent>(FName("ApplyDamage"));
	OverlapTrigger = CreateDefaultSubobject<UBoxComponent>(FName("BoxTrigger"));
	OverlapTrigger->SetupAttachment(GetRootComponent());
}

bool AWeaponOverlapDamage::ActivateItemCustom(const float DamageMult, const float DamageRangeMult)
{
	CurrentDamageMult = DamageMult;
	OverlapTrigger->SetBoundsScale(DamageRangeMult);
	OverlapTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnActorBeginOverlap);
	return true;
}

void AWeaponOverlapDamage::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		ApplyDamageComponent->MakeDamage(OtherActor, SweepResult, CurrentDamageMult);
	}
}

void AWeaponOverlapDamage::DeactivateItem()
{
	OverlapTrigger->OnComponentBeginOverlap.Clear();
}





