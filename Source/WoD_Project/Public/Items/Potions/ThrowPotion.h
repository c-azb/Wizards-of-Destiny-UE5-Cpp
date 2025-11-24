// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Potions/Potion.h"
#include "ThrowPotion.generated.h"

/**
 * 
 */
UCLASS()
class WOD_PROJECT_API AThrowPotion : public APotion
{
	GENERATED_BODY()

public:
	virtual void Destroyed() override;
protected:
	virtual bool ActivateItem() override;
	virtual EActivateItem GetActivateType() override;

private:
	UPROPERTY(EditAnywhere, Category = "Att_ThrowPotion") TObjectPtr<USoundBase> PotionHitSFX;
	UPROPERTY(EditAnywhere, Category = "Att_ThrowPotion") TObjectPtr<USoundBase> ThrowPotionSFX;
	UPROPERTY(EditAnywhere, Category = "Att_ThrowPotion") bool bTriggerOnHitPawn=false;
	
	UFUNCTION() void OnPotionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(NetMulticast, Reliable) void MulticastAddPotionForce(const FVector_NetQuantize ThrowForce, const FVector_NetQuantize StartPos);

	UFUNCTION() void TriggerPotion(const FRotator ImpactNormalRot);

};
