// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Spells/Spell.h"
#include "HoldSpell.generated.h"


UCLASS()
class WOD_PROJECT_API AHoldSpell : public ASpell
{
	GENERATED_BODY()

public:
	AHoldSpell();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() float CurrentDistance=0;
	UPROPERTY(EditAnywhere,Category="Att_HoldSpell") float GrowSpeed=1000;
	UPROPERTY(EditAnywhere, Category = "Att_HoldSpell") float MinOffsetFromWall = 0;
	UPROPERTY(Replicated) FVector CurrentServerEndPoint;
	FVector CurrentLocalEndPoint;
	//bool bSpellEndPointSet = false;

	void SetBeamParameters(const FRotator& SpellRot);
	void ProcessHoldSpell(const float DeltaSeconts);
};
