// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
//#include "GameTypes/ResourcesStruct.h"
#include "Characters/BaseCharacter.h"

void UResourcesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourcesComponent, CurrentMana);
	DOREPLIFETIME(UResourcesComponent, CurrentStamina);
	DOREPLIFETIME(UResourcesComponent, CurrentSpecial);
}

void UResourcesComponent::InicializeResource()
{
	Super::InicializeResource();
	CurrentMana = MaxResources.Mana;
	CurrentSpecial = 0;
	CurrentStamina = MaxResources.Stamina;
	OnRepCurrentResources();
}
bool UResourcesComponent::ConsumeResources(const FResourcesStruct& ConsumeCost,const float ResourcesMult)
{
	if (GetOwner()->HasAuthority() && CanConsumeResources(ConsumeCost))
	{
		CurrentMana -= ConsumeCost.Mana * ResourcesMult;
		CurrentSpecial -= ConsumeCost.Special * ResourcesMult;
		CurrentStamina -= ConsumeCost.Stamina * ResourcesMult;

		ReloadManaDelayTimer = ConsumeCost.Mana > .01f ? 1 : ReloadManaDelayTimer;
		ConsumeStaminaTime = ConsumeCost.Stamina > .01f ? 1 : ConsumeStaminaTime;

		OnRepCurrentResources();
		return true;
	}
	return false;
}

bool UResourcesComponent::ConsumeContinuousResources(const FResourcesStruct& ConsumeCost, float ResourcesMult, const float DeltaTime)
{
	ConsumeManaTime += DeltaTime * ConsumeManaSpeed;
	ResourcesMult *= DeltaTime * ConsumeManaTime* ConsumeManaTime;
	return ConsumeResources(ConsumeCost, ResourcesMult);
}

bool UResourcesComponent::CanConsumeResources(const FResourcesStruct& ConsumeCost)
{
	return CurrentMana >= ConsumeCost.Mana &&
		CurrentSpecial >= ConsumeCost.Special &&
		CurrentStamina >= ConsumeCost.Stamina;
}

void UResourcesComponent::AutoReloadMana(const float DeltaTime)
{
	if (ReloadManaDelayTimer <= .001f && CurrentMana < MaxResources.Mana)
	{
		ConsumeManaTime = 0;
		float Increment = DeltaTime * ReloadManaTime * ReloadManaTime;
		CurrentMana = FMath::Clamp(CurrentMana + Increment,0, MaxResources.Mana);
		ReloadManaTime = CurrentMana >= MaxResources.Mana ? 0 : ReloadManaTime + DeltaTime * ReloadManaSpeed;
		OnRepCurrentResources();
	}
	if(ReloadManaDelayTimer > 0)
	{
		ReloadManaDelayTimer = FMath::Clamp(ReloadManaDelayTimer - DeltaTime,0,1);
	}
}

void UResourcesComponent::AutoReloadStamina(const float DeltaTime)
{
	if (ConsumeStaminaTime <= .001f && CurrentStamina < MaxResources.Stamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + DeltaTime * ReloadStaminaSpeed, 0, MaxResources.Stamina);
		OnRepCurrentResources();
	}
	if (ConsumeStaminaTime > 0)
	{
		ConsumeStaminaTime = FMath::Clamp(ConsumeStaminaTime - DeltaTime, 0, 1);
	}
}

void UResourcesComponent::OnRepCurrentResources()
{
	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	}
	if (OwnerCharacter)
	{
		OwnerCharacter->OnRepResources();
	}
}

void UResourcesComponent::OnRepHealth()
{
	OnRepCurrentResources();
}

FResourcesStruct UResourcesComponent::GetCurrentResources()
{
	FResourcesStruct ResourcesStruct;
	ResourcesStruct.Mana = CurrentMana;
	ResourcesStruct.Stamina = CurrentStamina;
	ResourcesStruct.Special = CurrentSpecial;
	return ResourcesStruct;
}
