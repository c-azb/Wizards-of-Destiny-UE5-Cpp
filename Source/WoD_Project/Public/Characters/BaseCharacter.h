// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameTypes/WizardStates.h"
#include "GameTypes/PickUpEnums.h"

#include "BaseCharacter.generated.h"

class UNiagaraSystem;

UCLASS()
class WOD_PROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void OnRep_Controller() override;  //only client call
	virtual void PossessedBy(AController* NewController) override; //only server call

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	UFUNCTION() virtual bool IsAlive();
	UFUNCTION() virtual void OnRepResources();
	UFUNCTION() virtual void OnGetPickUp(const EPickUpType PickUpType, const uint32 PickUpId);

protected:

	UPROPERTY(EditAnywhere, Category = "Att_BaseCharacter") float WalkSpeed = 650;
	UPROPERTY(EditAnywhere, Category = "Att_BaseCharacter") float InCombatSpeed = 400;
	UPROPERTY(EditAnywhere, Category = "Att_BaseCharacter") float RunSpeedMult = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Att_Montages") TObjectPtr<UAnimMontage> GetHitHardMontage;

	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") TObjectPtr<UAnimMontage> VL_AttackNeutralMontage;
	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") uint32 VL_AttackNeutralSecsCount;
	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") TObjectPtr<UAnimMontage> VL_AttackSpellMontage;
	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") uint32 VL_AttackSepellSecsCount;
	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") TObjectPtr<UAnimMontage> VL_PainMontage;
	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") uint32 VL_PainSecsCount;

	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") TObjectPtr<UAnimMontage> VL_GrllsPainMontage;
	UPROPERTY(EditAnywhere, Category = "Att_VLMontages") TObjectPtr<UAnimMontage> VL_GrllsActionMontage;

	UPROPERTY(EditAnywhere, Category = "Att_BaseCharacter") TObjectPtr<UNiagaraSystem> BloodVFX;
	float SpawnBloodVFXTime=0;

	UFUNCTION() virtual void InitializeCharacter();

	void PlayMontage(TObjectPtr<UAnimMontage> AnimMontage, const int32 SectionCount, const bool bPlayRandom, const float StartTime = 0);
	void PlayVoiceLineAttackMontage(const bool bIncludeSpellVL);
	void PlayVoiceLinePainMontage();
	void PlayVoinceLineGrllsPain(const float ProbChange=2);
	void PlayVoinceLineGrllsAction(const float ProbChange=2);
	void EnableRagDoll();// (const FVector& HitPosition, const float DamageApplyed);

	void BaseOnGetDamage(const AActor* DamageCauser, const float DamageAmount);
	void SendKillEvent(const AController* InstigatorCtrl,const EWizardScore KillType);
	UFUNCTION() virtual void OnGetHitHard(const float DotProductDir);
	void PlayBloodVFX(FDamageEvent const& DamageEvent, const AActor* Instigator_);
};
