// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardPlayerController.generated.h"

class UGameplayWidget;
class UBagUserWidget;
struct FResourcesStruct;

UCLASS()
class WOD_PROJECT_API AWizardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetResourcesBars(const FResourcesStruct& ResourcesAmount, const float Health, const FResourcesStruct& MaxResourcesAmount, const float MaxHealth);
	void OnUpdateScore(const uint32 WizardLvl, const float WizardScorePercent);
	virtual bool InputKey(const FInputKeyParams& Params) override;
	void CloseBag();

private:
	UPROPERTY(EditAnywhere, Category = "Att_Widget") TSubclassOf<UGameplayWidget> GameplayWidgetClass;
	TObjectPtr<UGameplayWidget> GameplayWidget;

	UPROPERTY(EditAnywhere, Category = "Att_Widget") TSubclassOf<UBagUserWidget> BagWidgetClass;
	TObjectPtr<UBagUserWidget> BagWidget;

	void OpenBagInput();
	bool InicializeGameplayWidget();

	TObjectPtr<UUserWidget> AddWidget(UClass* WidgetClass);

	void GetWizardCharacterID();

};
