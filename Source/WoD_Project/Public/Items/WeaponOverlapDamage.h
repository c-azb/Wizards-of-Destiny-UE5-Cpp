// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/HoldItem.h"
#include "WeaponOverlapDamage.generated.h"


class UApplyDamageComponent;
class UShapeComponent;

UCLASS()
class WOD_PROJECT_API AWeaponOverlapDamage : public AHoldItem
{
	GENERATED_BODY()

public:
	AWeaponOverlapDamage();
	virtual bool ActivateItemCustom(const float DamageMult, const float DamageRangeMult) override;
	virtual void DeactivateItem() override;
private:
	float CurrentDamageMult = 1;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UApplyDamageComponent> ApplyDamageComponent;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UShapeComponent> OverlapTrigger;

	UFUNCTION() void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
