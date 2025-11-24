#pragma once

#include "ResourcesStruct.generated.h"

USTRUCT()
struct FResourcesStruct
{
	GENERATED_USTRUCT_BODY()
public:
	FResourcesStruct() { Mana = 0; Stamina = 0; Special = 0; };
	FResourcesStruct(float Mana_, float Stamina_, float Special_) { Mana = Mana_; Stamina = Stamina_; Special = Special_; };

	UPROPERTY(EditAnywhere) float Mana=0;
	UPROPERTY(EditAnywhere) float Stamina=0;
	UPROPERTY(EditAnywhere) float Special=0;
};

USTRUCT()
struct FBoostParameters
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere) float ManaEarn = 0;
	UPROPERTY(EditAnywhere) float StaminaEarn = 0;
	UPROPERTY(EditAnywhere) float SpecialEarn = 0;
	UPROPERTY(EditAnywhere) float HealthEarn = 0;

	UPROPERTY(EditAnywhere) float ExtraDamageBoost = 0;

	UPROPERTY(EditAnywhere) bool bInfinityMana = false;
	UPROPERTY(EditAnywhere) bool bInfinityHealth = false;
	UPROPERTY(EditAnywhere) bool bInfinityStamina = false;

	UPROPERTY(EditAnywhere) bool bOneHitKill = false;
	UPROPERTY(EditAnywhere) bool bOneHitDeath = false;


	UPROPERTY(EditAnywhere) float BoostTime = 0;

};