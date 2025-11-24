// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes/SpellEnums.h"
#include "GameTypes/WizardStates.h"
#include "GameTypes/ResourcesStruct.h"
#include "Items/ApplyDamageComponent.h"

#include "Spell.generated.h"

class UHealthComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class AMagicWand;
//class UApplyDamageComponent;
//struct FResourcesStruct;

UCLASS()
class WOD_PROJECT_API ASpell : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASpell();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION() virtual void StopSpell();

	UPROPERTY(EditAnywhere, Category = "Att_Spell") float MaxSpellDistance=500;
	UPROPERTY(EditAnywhere, Category = "Att_Spell") FResourcesStruct SpellCosts;
	UPROPERTY(EditAnywhere, Category = "Att_Spell") uint32 MinWizardLvl=0;
	EActivateItem ActivateItemType = EActivateItem::EAI_None;

protected:

	UPROPERTY(EditAnywhere, Category = "Att_Spell") float DestroyDelay=1;
	UPROPERTY(EditAnywhere, Category = "Att_Spell") TObjectPtr<USoundBase> SpellSound;

	UPROPERTY() TObjectPtr<UAudioComponent> SpellAudioComponent;

	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> SpellMesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UHealthComponent> SpellHealth;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> SpellNiagara;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UApplyDamageComponent> ApplyDamageComponent;

	UPROPERTY(ReplicatedUsing = OnRepIsDestroying)bool IsDestroying = false;

	void SpawnSpellVFX(TObjectPtr<UNiagaraSystem> SpellVFX,const TObjectPtr<USceneComponent> AttachTo = nullptr,const FHitResult* Normal = nullptr, const bool AttachKeepingPos=false);

	UPROPERTY(EditAnywhere,Category="Att_Spell") TObjectPtr<UNiagaraSystem> CastSpellVFX;
	UPROPERTY(EditAnywhere,Category="Att_Spell") TObjectPtr<UNiagaraSystem> HitSpellVFX;
	UPROPERTY(EditAnywhere,Category="Att_Spell") TObjectPtr<UNiagaraSystem> DeathSpellVFX;

	UPROPERTY() TObjectPtr<AMagicWand> MagicWandRef;

	float SpawnHitVFXCounter = 0;

	UFUNCTION() virtual void SpellDeath();

	void PlaySpellDeathCosmetics();
	UFUNCTION() void DestroySpell();

	FVector GetWandPointLoc();
	bool GetMWAimTargetPos(const float CurrentReach, FHitResult* OutHitResult, FRotator* OutRot, FVector* OutPos);

	UFUNCTION() virtual void OnRepIsDestroying();

};
