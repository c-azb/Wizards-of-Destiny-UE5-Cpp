// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTypes/WizardStates.h"
#include "BagUserWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UUniformGridPanel;
class UUniformGridSlot;
class UInventoryItemWidget;
class AWizardPlayerState;

UCLASS()
class WOD_PROJECT_API UBagUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBagUserWidget(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> MWTabBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> PotionTabBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> GrocerysTabBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> FBTabBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> RelicsTabBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> PageRightBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> PageLeftBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> PagesText;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> OkBtn;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UUniformGridPanel> SlotsGridPanel;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UUniformGridPanel> ItemsGridPanel;
	
	UPROPERTY(EditAnywhere, Category = "Att_Grid") TObjectPtr<UTexture2D> SlotTextureBG;
	UPROPERTY(EditAnywhere, Category = "Att_Grid") TObjectPtr<UTexture2D> SlotLockedTexture;
	UPROPERTY(EditAnywhere, Category = "Att_Grid") TObjectPtr<UTexture2D> AddIconTexture;

	TArray<UImage*> GridSlotImages;
	TArray<UInventoryItemWidget*> GridItemsWidgets;
	EHoldItem CurrentInventoryTab = EHoldItem::EHI_MagicWand;
	uint32 CurrentInventoryPage = 0; 

	UPROPERTY(EditAnywhere,Category="Att_Grid") int32 GridRows = 3;
	UPROPERTY(EditAnywhere, Category = "Att_Grid") int32 GridColumns = 3;
	UPROPERTY(EditAnywhere, Category = "Att_Grid") TSubclassOf<UInventoryItemWidget> ItemWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Att_Grid") FVector2D ImgSize = FVector2D(220,220);

	AWizardPlayerState* WizardPlayerState=nullptr;

	TObjectPtr<UInventoryItemWidget> CreateNewGridItem(const FString& ImageName, UTexture2D* BaseTexture, const FVector2D& ImageSize);
	TObjectPtr<UImage> CreateNewSlotImg(const FString& ImageName, UTexture2D* BaseTexture, const FVector2D& ImageSize);
	
	UFUNCTION() void OnClickOkBtn();
	UFUNCTION() void OnSelectMWTab();
	UFUNCTION() void OnSelectPotionTab();
	UFUNCTION() void OnSelectFBTab();
	UFUNCTION() void OnSelectGrocerysTab();
	UFUNCTION() void OnSelectRelicsTab();
	void OnClickButtonTab(const EHoldItem NewInventoryTab);
	UFUNCTION() void OnClickItemWidget(const uint32 ItemId, EInventoryItemState CurrItemState);

	void OnClickRightPage();
	void OnClickLeftPage();
	void ChangePage(int8 Increment);

	uint32 GetPagesCount();
	void SetupPage();

};
