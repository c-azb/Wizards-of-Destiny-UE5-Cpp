// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WOD_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual void InicializeResource();
	void GetDamage(const float Damage);
	bool IsAlive();

protected:
	UPROPERTY(EditAnywhere, Category = "Att_Health") float MaxHealth = 100;
	UPROPERTY(ReplicatedUsing=OnRepHealth) float Health = 1;
	UFUNCTION() virtual void OnRepHealth();
};
