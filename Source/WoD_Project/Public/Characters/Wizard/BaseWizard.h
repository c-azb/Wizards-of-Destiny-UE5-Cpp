// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GameTypes/WizardStates.h"
//#include "GameTypes/ResourcesStruct.h"

#include "BaseWizard.generated.h"

class UResourcesComponent;
class UWizardItemsComponent;
struct FResourcesStruct;
class AWizardPlayerState;

UCLASS()
class WOD_PROJECT_API ABaseWizard : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseWizard();
	//virtual void OnRep_Controller() override;  //only client call
	//virtual void PossessedBy(AController* NewController) override; //only server call
	//virtual void OnRep_PlayerState() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool IsAlive() override;
	
public:
	UPROPERTY(ReplicatedUsing=OnRepWizardState) EWizardState WizardState = EWizardState::EWCS_Combat;
	
	void OnRepComboDir(uint8 ComboDir);
	void OnSpellComboTimeout(const bool bActivateItemOnTimeOut);

	UPROPERTY(ReplicatedUsing=OnRepIsRunning) bool IsRunning = false;
	UPROPERTY(ReplicatedUsing = OnRepIsDashing) bool IsDashing = false;

	void ActivateItem();
	void DeactivateItem(const bool bCancel = false);
	void ChangeItem();
	void EndChangeItem();
	void StartChangeItem(const uint32 ItemId, const EHoldItem ItemType);

	float GetMaxSpeed();
	uint32 GetCurrentItemHandHoldId();

	void EndDash();
	void EndGetHit();
	bool IsGettingHitHard();

protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UResourcesComponent> ResourcesComponent;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UWizardItemsComponent> ItemsComponent;
	
	virtual void InitializeCharacter() override;
	//UFUNCTION() virtual void InicializeWizard();
	void InitializeItems(const uint32 MWCode);
	void AddSpellCombo(const ESpellComboDir ComboDir);
	EActivateItem SetupItemActivation();
	void StartActivateItem();
	void SetupMovementType();
	void SetWizardSpeed();
	void PlayMontageBasedOnState();
	void PlayComboMontage(const ESpellComboDir ComboDir);
	bool IsChagingItem();
	bool IsActivatingItem();
	bool CanMove();
	void OnWizardDead();
	void Dash();
	void Run(const bool bIsRunClick);
	virtual void OnGetHitHard(const float DotProductDir) override;
	uint32 GetWizardLevel();
	void SetWizardState(const EWizardState WizardState_);

private:
	float MaxFlySpeed = 1;

	UPROPERTY(EditAnywhere,Category="Att_BaseWizard") float RunStaminaCost=100;
	UPROPERTY(EditAnywhere,Category="Att_BaseWizard") float DashStaminaCost=30;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> ThrowSpellMontage;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> HoldSpellMontage;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> ChangeItem_Basic;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> ChangeItem_FlyingBroom;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> ComboDirMontage;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> DashMontage;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> ThrowPotionMontage;
	UPROPERTY(EditAnywhere,Category="Att_Montages") TObjectPtr<UAnimMontage> DrinkPotionMontage;

	AWizardPlayerState* WizardPlayerState;

	bool GetWizardPlayerState();
	int32 GetDefaultEquippedItem(const EHoldItem ItemType);
	void SetDefaultEquippedItem(const EHoldItem ItemType,const int32 ItemId);

	void ProcessContinuousResources(const float DeltaTime);
	void SetFlyingSpeedBasedOnGround(const float DeltaTime);
	bool CanMakeAction(const EWizardState NewWizardState, const bool bIncludeEqual = false);
	UFUNCTION() void OnRepWizardState(EWizardState LastWizardState);
	UFUNCTION() void OnRepIsRunning();
	UFUNCTION() void OnRepIsDashing();
};
