// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Wizard/BaseWizard.h"
#include "Wizard.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class USpringArmComponent;
class UCameraComponent;

class AWizardPlayerController;

class UOverlapInteractionsComponent;

UCLASS()
class WOD_PROJECT_API AWizard : public ABaseWizard
{
	GENERATED_BODY()

public:
	AWizard();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRepResources() override;

	void ChangeItemInventory(const uint32 ItemId, const EHoldItem ItemType);

protected:
	virtual void InitializeCharacter() override;

private:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UOverlapInteractionsComponent> InteractionsComponent;
	
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputMappingContext> IMC_BasicControlls;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_Movement;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_LookAround;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_Run;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_SpellCombo;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_ActivateItem;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_ChangeItem;
	UPROPERTY(EditAnywhere,Category="Att_Inputs") TObjectPtr<UInputAction> IA_Dash;

	UPROPERTY(VisibleAnywhere) TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> Camera;
	
	bool bIsClick = true;//controlls if mouse event is pressed or released without sent throw RPC
	bool bIsRunClick = false;//controlls if mouse event is pressed or released without sent throw RPC

	AWizardPlayerController* WizardPC = nullptr;
	bool GetWizardPC();

	void MovementInput(const FInputActionValue& InputValue);
	void LookAroundInput(const FInputActionValue& InputValue);
	void RunInput(const FInputActionValue& InputValue);
	void SpellComboInput(const FInputActionValue& InputValue);
	void ActivateItemInput(const FInputActionValue& InputValue);

	void ChangeItemInput(const FInputActionValue& InputValue); 

	void DashInput(const FInputActionValue& InputValue);

	
	UFUNCTION(Server, Reliable) void ServerChangeItem(const uint32 ItemId, const EHoldItem ItemType);
	UFUNCTION(Server,Reliable) void ServerSpellCombo(const ESpellComboDir ComboDir);
	UFUNCTION(Server,Reliable) void ServerActivateItem();
	UFUNCTION(Server,Reliable) void ServerInicializeItem(const uint32 MWCode);
	UFUNCTION(Server,Reliable) void ServerDash();
	UFUNCTION(Server,Reliable) void ServerRun();

	void SetUIResources();
};
