// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Wizard/Wizard.h"
#include "Characters/Wizard/OverlapInteractionsComponent.h"

#include "EnhancedInputSubsystems.h"	
#include "EnhancedInputComponent.h" 
#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Characters/Wizard/WizardItemsComponent.h"
#include "Characters/Wizard/WizardPlayerController.h"
#include "Characters/Wizard/ResourcesComponent.h"

#include "GameManagers/WoDGameInstance.h"


AWizard::AWizard()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	InteractionsComponent = CreateDefaultSubobject<UOverlapInteractionsComponent>(FName("InteractionsComponent"));
	InteractionsComponent->SetIsReplicated(true);
}

void AWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &ThisClass::MovementInput);
		EnhancedInput->BindAction(IA_LookAround, ETriggerEvent::Triggered, this, &ThisClass::LookAroundInput);
		
		EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

		EnhancedInput->BindAction(IA_Run, ETriggerEvent::Triggered, this, &ThisClass::RunInput);

		EnhancedInput->BindAction(IA_SpellCombo, ETriggerEvent::Triggered, this, &ThisClass::SpellComboInput);
		
		EnhancedInput->BindAction(IA_ActivateItem, ETriggerEvent::Triggered, this, &ThisClass::ActivateItemInput);
		
		EnhancedInput->BindAction(IA_ChangeItem, ETriggerEvent::Triggered, this, &ThisClass::ChangeItemInput);
		
		EnhancedInput->BindAction(IA_Dash, ETriggerEvent::Triggered, this, &ThisClass::DashInput);
	}
}

void AWizard::InitializeCharacter()
{
	Super::InitializeCharacter();

	if (HasAuthority())
	{
		InteractionsComponent->SetupOverlapEvent(GetCapsuleComponent());
	}

	if (IsLocallyControlled())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(IMC_BasicControlls, 0);
			}
		}

		if (UWoDGameInstance* WoDGameInstance = GetWorld()->GetGameInstance<UWoDGameInstance>())
		{
			uint32 MWCode = WoDGameInstance->GetEquipedItem(EHoldItem::EHI_MagicWand);
			ServerInicializeItem(MWCode);
		}
	}
}

void AWizard::ServerInicializeItem_Implementation(const uint32 MWCode)
{
	InitializeItems(MWCode);
}


void AWizard::MovementInput(const FInputActionValue& InputValue)
{
	if (!CanMove()) return;

	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (Controller)
	{
		FRotator ControllerRotation = Controller->GetControlRotation();
		ControllerRotation.Roll = 0;
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		{
			//A and D rotate on flying mode
			InputVector.Y = FMath::Clamp(InputVector.Y, 0, 1);
			FVector2D LookInput = InputVector;
			LookInput.Y = 0;
			LookAroundInput(LookInput);
			InputVector.X = 0;	
		}
		else
			ControllerRotation.Pitch = 0;

		FVector ForwardDir = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		FVector RightDir = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir,InputVector.Y);
		AddMovementInput(RightDir,InputVector.X);
		
		//check if is on first person / third person also
		//GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->IsFalling();
		
	}
	


}

void AWizard::LookAroundInput(const FInputActionValue& InputValue)
{
	if (Controller)
	{
		FVector2D LookAxis = InputValue.Get<FVector2D>();
		AddControllerYawInput(LookAxis.X);
		AddControllerPitchInput(LookAxis.Y);
	}
}

void AWizard::RunInput(const FInputActionValue& InputValue)
{
	ServerRun();
}

void AWizard::ServerRun_Implementation()
{
	bIsRunClick = !bIsRunClick;
	Run(bIsRunClick);
}

void AWizard::SpellComboInput(const FInputActionValue& InputValue)
{
	FVector2D ComboDir = InputValue.Get<FVector2D>();
	ESpellComboDir ComboDirUnpack;

	if (ComboDir.X == 1) ComboDirUnpack = ESpellComboDir::ESCD_Right;
	else if (ComboDir.X == -1) ComboDirUnpack = ESpellComboDir::ESCD_Left;
	else if (ComboDir.Y == 1) ComboDirUnpack = ESpellComboDir::ESCD_Up;
	else if (ComboDir.Y == -1) ComboDirUnpack = ESpellComboDir::ESCD_Down;
	else return;

	ServerSpellCombo(ComboDirUnpack);
	
}

void AWizard::ServerSpellCombo_Implementation(const ESpellComboDir ComboDir)
{
	AddSpellCombo(ComboDir);
}

void AWizard::ActivateItemInput(const FInputActionValue& InputValue)
{
	//if (ItemsComponent->IsSpellComboLocallyValid())
	{
		ServerActivateItem();
	}
}

void AWizard::ServerActivateItem_Implementation()
{
	EActivateItem ActivateType = EActivateItem::EAI_None;
	if (bIsClick)
	{
		ActivateType = SetupItemActivation();
	}
	else
	{
		ActivateType = ItemsComponent->GetItemActivateType();
	}

	if (bIsClick && (ActivateType == EActivateItem::EAI_HoldSpell || ActivateType == EActivateItem::EAI_DrinkPotion))
	{
		StartActivateItem();
	}
	else if (!bIsClick && ActivateType == EActivateItem::EAI_HoldSpell)
	{
		DeactivateItem();
	}
	else if (!bIsClick)
	{
		StartActivateItem();
	}
	bIsClick = !bIsClick;
}

/*TEST CHANGE FOR INVENTORY SYSTEM*/
void AWizard::ChangeItemInput(const FInputActionValue& InputValue)
{
	float Value_ = InputValue.Get<float>();

	if (Value_ > 0)   //Hardcoding for TESTS
	{ 
		ServerChangeItem(0, EHoldItem::EHI_MagicWand);
	}
	else
	{
		//ServerChangeItem(0, EHoldItem::EHI_Potion);
		ServerChangeItem(0, EHoldItem::EHI_FlyingBroom);
	}
}

void AWizard::ChangeItemInventory(const uint32 ItemId, const EHoldItem ItemType)
{
	ServerChangeItem(ItemId, ItemType);
}

void AWizard::ServerChangeItem_Implementation(const uint32 ItemId, const EHoldItem ItemType)
{
	StartChangeItem(ItemId, ItemType);
}

void AWizard::DashInput(const FInputActionValue& InputValue)
{
	ServerDash();
}

void AWizard::ServerDash_Implementation()
{
	Dash();
}

void AWizard::OnRepResources()
{
	Super::OnRepResources();
	if (IsLocallyControlled())
	{
		SetUIResources();
	}
}

bool AWizard::GetWizardPC()
{
	if (WizardPC == nullptr)
	{
		WizardPC = Cast<AWizardPlayerController>(GetController());
	}
	return WizardPC != nullptr;
}

void AWizard::SetUIResources()
{
	if (GetWizardPC())
	{
		WizardPC->SetResourcesBars(ResourcesComponent->GetCurrentResources(), ResourcesComponent->GetCurrentHealth(), ResourcesComponent->GetMaxResources(), ResourcesComponent->GetMaxHealth());
	}
}