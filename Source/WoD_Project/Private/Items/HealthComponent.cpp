// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent,Health);
}

void UHealthComponent::InicializeResource()
{
	if(GetOwner()->HasAuthority()) Health = MaxHealth;
}

void UHealthComponent::GetDamage(const float Damage)
{
	if (GetOwner()->HasAuthority())
	{
		Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
		OnRepHealth();
	}
}

bool UHealthComponent::IsAlive()
{
	return Health > 0;
}

void UHealthComponent::OnRepHealth(){}




