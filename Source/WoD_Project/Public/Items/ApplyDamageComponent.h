// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApplyDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WOD_PROJECT_API UApplyDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UApplyDamageComponent();

protected:
	virtual void BeginPlay() override;

public:	
	bool MakeDamage(AActor* OtherActor, const FHitResult& HitResult, const float DamageMult);

private:
	UPROPERTY(EditAnywhere, Category = "Att_Damage") float Damage = 0;
	UPROPERTY(EditAnywhere, Category = "Att_Damage") bool DamageSpells = true;
	UPROPERTY(EditAnywhere, Category = "Att_Damage") bool DamageCharacters = true;

	UPROPERTY(EditAnywhere, Category = "Att_RadialDamage") bool ApplyRadialDamage = false;
	UPROPERTY(EditAnywhere, Category = "Att_RadialDamage") float InnerRadius = 50;
	UPROPERTY(EditAnywhere, Category = "Att_RadialDamage") float OutterRadius = 200;
	UPROPERTY(EditAnywhere, Category = "Att_RadialDamage") float DamageFalloff = 1;
	UPROPERTY(EditAnywhere, Category = "Att_RadialDamage") float MinimunDamage = 0;

	UPROPERTY() TObjectPtr<AController> DamageInstigator;
	
	void MakePointDamage(AActor* OtherActor, const FHitResult& HitResult, const float DamageMult);
	void MakeRadialDamage(AActor* OtherActor, const FHitResult& HitResult, const float DamageMult);
	bool CanApplyDamage(AActor* OtherActor);

		
};
