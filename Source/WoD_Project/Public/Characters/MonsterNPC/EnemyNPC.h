// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GameTypes/EnemyEnums.h"
#include "GameTypes/PickUpEnums.h"
#include "EnemyNPC.generated.h"

class AHoldItem;
class AAIEnemyController;
class UHealthComponent;

UCLASS()
class WOD_PROJECT_API AEnemyNPC : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyNPC();

protected:
	virtual void InitializeCharacter() override;

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool IsAlive() override;
	bool IsGettingHitHard();

	bool SetEnemyState(const EEnemyState NewEnemyState);
	void SetMovementState();

	void SetupAttack(const bool bJumpAtk);
	void Attack(const float DamageMult,const float DamageRangeMult);
	void EndAttack();
	void EndGetHitHard();

	bool GetActorsInAtkRange(TArray<FOverlapResult>& OutActors, const EEnemyState AttackType);

	float GetMaximunSpeed();

	UPROPERTY(ReplicatedUsing=OnRep_EnemyState)EEnemyState EnemyState = EEnemyState::EES_None;

	UPROPERTY(EditAnywhere, Category = "Att_EnemyNPC") FVector3f CloseAtkBoxSize = FVector3f(100,15,15);
	UPROPERTY(EditAnywhere, Category = "Att_EnemyNPC") FVector3f FarAtkBoxSize = FVector3f(100,15,15);
	UPROPERTY(EditAnywhere, Category = "Att_EnemyNPC") float FarAttackRange = 0;
	UPROPERTY(EditAnywhere, Category = "Att_EnemyNPC") float FarAttackProbability = 0.5f;

private:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere,Category="Att_EnemyNPC") TSubclassOf<AHoldItem> EnemyWeapon;
	TObjectPtr<AHoldItem> EnemyWeaponInst;

	TObjectPtr< AAIEnemyController> AIEnemyController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Att_EnemyNPC") TArray<EGroceryType> DropGrocerysOptions;

	UPROPERTY(EditAnywhere, Category = "Att_Montages") TObjectPtr<UAnimMontage> CloseAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Att_Montages") uint32 CloseAttacksCount=1;
	UPROPERTY(EditAnywhere, Category = "Att_Montages") TObjectPtr<UAnimMontage> FarAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Att_Montages") uint32 FarAttacksCount = 1;

	void PlayMontageBasedOnState();
	virtual void OnGetHitHard(float DotProductDir) override;
	bool GetAIController();
	bool IsAttacking();
	void DropGrocery();

	UFUNCTION() void EndDeathTimer();
	UFUNCTION()void OnRep_EnemyState();
};
