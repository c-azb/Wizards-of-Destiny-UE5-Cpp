// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Spells/Spell.h"
#include "ThrowSpell.generated.h"

class UProjectileMovementComponent;

UCLASS()
class WOD_PROJECT_API AThrowSpell : public ASpell
{
	GENERATED_BODY()

public:
	AThrowSpell();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void StopSpell() override;

protected:
	virtual void BeginPlay() override;
	virtual void SpellDeath() override;

private:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UProjectileMovementComponent> ProjectileComponent;

	UPROPERTY(EditAnywhere, Category = "Att_ThrowSpell") bool AutoDestroyOnHit = true;
	UPROPERTY(EditAnywhere, Category = "Att_ThrowSpell") float SpellLifeTime = 7;

	FVector StartPosition;
	UPROPERTY(EditAnywhere, Category = "Att_ThrowSpell") float StartScale = 1; //needs to set or replicate...

	UPROPERTY(ReplicatedUsing = OnRepFinalPos) FVector_NetQuantize FinalSpellPos;

	void CheckSelfOverlapping();

	UFUNCTION() void OnHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION() void OnRepFinalPos();
	virtual void OnRepIsDestroying() override;

};
