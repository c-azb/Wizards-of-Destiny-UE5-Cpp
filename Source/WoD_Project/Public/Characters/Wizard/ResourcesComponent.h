// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/HealthComponent.h"
#include "GameTypes/ResourcesStruct.h"

#include "ResourcesComponent.generated.h"

//struct FResourcesStruct;
class ABaseCharacter;

UCLASS()
class WOD_PROJECT_API UResourcesComponent : public UHealthComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void InicializeResource() override;
	bool ConsumeResources(const FResourcesStruct& ConsumeCost, const float ResourcesMult);
	bool ConsumeContinuousResources(const FResourcesStruct& ConsumeCost, float ResourcesMult, const float DeltaTime);
	bool CanConsumeResources(const FResourcesStruct& ConsumeCost);
	void AutoReloadMana(const float DeltaTime);
	void AutoReloadStamina(const float DeltaTime);

protected:
	UPROPERTY(EditAnywhere, Category = "Att_Resources") FResourcesStruct MaxResources;
	UPROPERTY(ReplicatedUsing=OnRepCurrentResources) float CurrentMana;
	UPROPERTY(ReplicatedUsing=OnRepCurrentResources) float CurrentStamina;
	UPROPERTY(ReplicatedUsing=OnRepCurrentResources) float CurrentSpecial;

	ABaseCharacter* OwnerCharacter = nullptr;

	UFUNCTION() void OnRepCurrentResources();
	virtual void OnRepHealth() override;
	
private:
	float ReloadManaDelayTimer = 0;
	float ReloadManaTime = 0;
	UPROPERTY(EditAnywhere, Category = "Att_Resources") float ReloadManaSpeed = .2f;

	float ConsumeManaTime = 0;
	UPROPERTY(EditAnywhere, Category = "Att_Resources") float ConsumeManaSpeed = 1;

	float ConsumeStaminaTime = 0;
	UPROPERTY(EditAnywhere, Category = "Att_Resources") float ReloadStaminaSpeed = 3.5f;

public:
	FResourcesStruct GetCurrentResources();
	FORCEINLINE FResourcesStruct GetMaxResources() const { return MaxResources; };
	FORCEINLINE float GetCurrentHealth() const { return Health; };
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; };
};
