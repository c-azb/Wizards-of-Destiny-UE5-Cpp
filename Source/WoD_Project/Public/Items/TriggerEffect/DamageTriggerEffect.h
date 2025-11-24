// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"

#include "DamageTriggerEffect.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UApplyDamageComponent;

UCLASS()
class WOD_PROJECT_API ADamageTriggerEffect : public AActor,public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

public:
	ADamageTriggerEffect();
	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> EffectNiagara;
	UPROPERTY(VisibleAnywhere) TObjectPtr<USphereComponent> SphereCollider;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UApplyDamageComponent> ApplyDamageComponent;

	UPROPERTY(EditAnywhere,Category = "Att_DamageTriggerEffect") TObjectPtr<USoundBase> EffectSoundSFX;
	UPROPERTY(EditAnywhere,Category = "Att_DamageTriggerEffect") bool bApplyTickDamage = true;
	UPROPERTY(EditAnywhere,Category = "Att_DamageTriggerEffect") bool bUseNiagaraCollision = false;
	UPROPERTY(EditAnywhere,Category = "Att_DamageTriggerEffect") FVector3f NiagaraCollSize = FVector3f(50,50,50);

	UPROPERTY() UAudioComponent* AudioComponent=nullptr;

	UFUNCTION() void OnEffectFinished(UNiagaraComponent* PSystem);
};
