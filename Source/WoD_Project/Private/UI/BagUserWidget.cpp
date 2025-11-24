// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BagUserWidget.h"
#include "Components\Image.h"
#include "Components\TextBlock.h"
#include "Components\UniformGridPanel.h"
#include "Components\UniformGridSlot.h"
#include "Components\Button.h"
#include "Blueprint/WidgetTree.h"
#include "UI/SubWidget/InventoryItemWidget.h"

#include "GameTypes/InventoryStructs.h"
#include "GameTypes/UIInventoryEnums.h"
#include "GameTypes/BaseItemStruct.h"

#include "UObject/ConstructorHelpers.h"

#include "GameManagers/WoDGameInstance.h"

#include "Characters/Wizard/Wizard.h"
#include "Characters/Wizard/WizardPlayerController.h"
#include "Characters/Wizard/WizardPlayerState.h"

UBagUserWidget::UBagUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> LockerImg(TEXT("/Game/UI/Images/Icons/Locker"));
	if(LockerImg.Object != nullptr) SlotLockedTexture= LockerImg.Object;
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> BaseSlotImg(TEXT("/Game/UI/Images/Panels/InventorySlotBG"));
	if (BaseSlotImg.Object != nullptr) SlotTextureBG = BaseSlotImg.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> BaseAddIconImg(TEXT("/Game/UI/Images/Icons/AddIcon"));
	if (BaseAddIconImg.Object != nullptr) AddIconTexture = BaseAddIconImg.Object;
}

bool UBagUserWidget::Initialize()
{
	if (!Super::Initialize()) return false;
	if(!SlotTextureBG || !SlotLockedTexture || !ItemWidgetClass || !RelicsTabBtn || !OkBtn) return true;
	
	FString SlotName,ItemName;
	for (int i = 0; i < GridRows*GridColumns; i++)
	{
		SlotName = FString::Printf(TEXT("Slot_%d"), i);
		ItemName = FString::Printf(TEXT("Item_%d"), i);
		TObjectPtr<UImage> SlotImage = CreateNewSlotImg(SlotName,SlotTextureBG,ImgSize);
		TObjectPtr<UInventoryItemWidget> ItemImage = CreateNewGridItem(ItemName, SlotLockedTexture, ImgSize);
		
		SlotsGridPanel->AddChildToUniformGrid(SlotImage, i / GridRows, i % GridColumns);
		ItemsGridPanel->AddChildToUniformGrid(ItemImage, i / GridRows, i % GridColumns);
		GridSlotImages.Add(SlotImage);
		GridItemsWidgets.Add(ItemImage);
	}

	OnSelectMWTab(); // inicialize tabs and pages as MW list

	MWTabBtn->OnClicked.AddDynamic(this, &ThisClass::OnSelectMWTab);
	PotionTabBtn->OnClicked.AddDynamic(this, &ThisClass::OnSelectPotionTab);
	FBTabBtn->OnClicked.AddDynamic(this, &ThisClass::OnSelectFBTab);
	GrocerysTabBtn->OnClicked.AddDynamic(this, &ThisClass::OnSelectGrocerysTab);
	RelicsTabBtn->OnClicked.AddDynamic(this, &ThisClass::OnSelectRelicsTab);

	PageLeftBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickLeftPage);
	PageRightBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickRightPage);
	
	OkBtn->OnClicked.AddDynamic(this, &ThisClass::OnClickOkBtn);

	return true;
}

TObjectPtr<UInventoryItemWidget> UBagUserWidget::CreateNewGridItem(const FString& ImageName, UTexture2D* BaseTexture,const FVector2D& ImageSize)
{
	TObjectPtr<UInventoryItemWidget> ItemWidget = WidgetTree->ConstructWidget<UInventoryItemWidget>(ItemWidgetClass, FName(ImageName));
	
	if (ItemWidget && ItemWidget->ItemImg)
	{
		FSlateBrush Brush = ItemWidget->ItemImg->GetBrush();
		Brush.SetResourceObject(Cast<UObject>(BaseTexture));
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.SetImageSize(ImageSize);
		ItemWidget->ItemImg->SetBrush(Brush);
	}
	return ItemWidget;
}

TObjectPtr<UImage> UBagUserWidget::CreateNewSlotImg(const FString& ImageName, UTexture2D* BaseTexture, const FVector2D& ImageSize)
{
	TObjectPtr<UImage> ItemWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), FName(ImageName));
	if (ItemWidget)
	{
		FSlateBrush Brush = ItemWidget->GetBrush();
		Brush.SetResourceObject(Cast<UObject>(BaseTexture));
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.SetImageSize(ImageSize);
		ItemWidget->SetBrush(Brush);
	}
	return ItemWidget;
}

void UBagUserWidget::OnClickOkBtn()
{
	if (AWizardPlayerController* WizardPC = GetOwningPlayer<AWizardPlayerController>())
	{
		WizardPC->CloseBag();
	}
}

void UBagUserWidget::OnSelectMWTab()
{
	OnClickButtonTab(EHoldItem::EHI_MagicWand);
}

void UBagUserWidget::OnSelectPotionTab()
{
	OnClickButtonTab(EHoldItem::EHI_Potion);
}

void UBagUserWidget::OnSelectFBTab()
{
	OnClickButtonTab(EHoldItem::EHI_FlyingBroom);
}

void UBagUserWidget::OnSelectGrocerysTab()
{
	OnClickButtonTab(EHoldItem::EHI_Grocery);
}

void UBagUserWidget::OnSelectRelicsTab()
{
	OnClickButtonTab(EHoldItem::EHI_Relics);
}

void UBagUserWidget::OnClickButtonTab(const EHoldItem NewInventoryTab)
{
	MWTabBtn->SetIsEnabled(EHoldItem::EHI_MagicWand != NewInventoryTab);
	PotionTabBtn->SetIsEnabled(EHoldItem::EHI_Potion != NewInventoryTab);
	GrocerysTabBtn->SetIsEnabled(EHoldItem::EHI_Grocery != NewInventoryTab);
	FBTabBtn->SetIsEnabled(EHoldItem::EHI_FlyingBroom != NewInventoryTab);
	RelicsTabBtn->SetIsEnabled(EHoldItem::EHI_Relics != NewInventoryTab);

	CurrentInventoryTab = NewInventoryTab;

	ChangePage(0);
}

void UBagUserWidget::OnClickRightPage()
{
	ChangePage(1);
}

void UBagUserWidget::OnClickLeftPage()
{
	ChangePage(-1);
}

void UBagUserWidget::ChangePage(int8 Increment)
{
	if (Increment == 0)
	{
		CurrentInventoryPage = 0;
	}

	uint32 MaximunPages = GetPagesCount();

	CurrentInventoryPage = FMath::Clamp(CurrentInventoryPage + Increment, 0, MaximunPages);
	PageLeftBtn->SetIsEnabled(CurrentInventoryPage > 0);
	PageRightBtn->SetIsEnabled(CurrentInventoryPage < MaximunPages);

	FText PageText = FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentInventoryPage + 1, MaximunPages + 1));
	PagesText->SetText(PageText);

	SetupPage();
}

uint32 UBagUserWidget::GetPagesCount()
{
	if (UWoDGameInstance* WoDGameInstance = GetWorld()->GetGameInstance<UWoDGameInstance>())
	{
		uint8 SlotsCount;

		if (CurrentInventoryTab == EHoldItem::EHI_MagicWand || CurrentInventoryTab == EHoldItem::EHI_FlyingBroom)
		{
			TArray<FItemInventory> CustomItems;
			WoDGameInstance->GetInventoryItems(EInventoryType::EIT_Bag, CurrentInventoryTab, CustomItems, SlotsCount);
			return CustomItems.Num() / (GridRows * GridColumns);
		}
		else
		{
			TMap<uint32, uint32> Items;
			WoDGameInstance->GetInventoryItems(EInventoryType::EIT_Bag, CurrentInventoryTab, Items, SlotsCount);
			return Items.Num() / (GridRows * GridColumns);
		}
	}
	return 0;
}

void UBagUserWidget::SetupPage()
{
	if (UWoDGameInstance* WoDGameInstance = GetWorld()->GetGameInstance<UWoDGameInstance>())
	{
		uint8 SlotsCount=0;
		//TMap<uint32, uint32> Items;
		TArray<FBagInfo> Items;

		if (CurrentInventoryTab == EHoldItem::EHI_MagicWand || CurrentInventoryTab == EHoldItem::EHI_FlyingBroom)
		{
			TArray<FItemInventory> CustomItems;
			WoDGameInstance->GetInventoryItems(EInventoryType::EIT_Bag, CurrentInventoryTab, CustomItems, SlotsCount);
			for (const FItemInventory Item : CustomItems)
			{
				FBagInfo BagItem;
				BagItem.ItemAmount = 1;
				BagItem.ItemId = Item.ItemCode;
				Items.Add(BagItem);
			}
		}
		else
		{
			if (WizardPlayerState == nullptr)
			{
				WizardPlayerState = GetOwningPlayerState<AWizardPlayerState>();
			}
			if (WizardPlayerState)
			{
				Items = WizardPlayerState->GetPlayerItems(CurrentInventoryTab);
			}
		}

		int32 CurrentIndex = CurrentInventoryPage * GridRows * GridColumns;

		for (int i = 0; i < GridRows * GridColumns; i++)
		{
			UInventoryItemWidget* ItemWidget = GridItemsWidgets[i];

			if (Items.Num() > CurrentIndex)
			{
				FBaseItem* BaseItem = WoDGameInstance->SearchItem(Items[CurrentIndex].ItemId, CurrentInventoryTab);
				ItemWidget->ItemImg->SetBrushResourceObject(Cast<UObject>(BaseItem->ItemImg));
				ItemWidget->ItemId = Items[CurrentIndex].ItemId;// Items[ItemsCodes[CurrentIndex]];
				ItemWidget->SetItemState(EInventoryItemState::EIIS_Occupied);
				ItemWidget->ItemName->SetText(FText::FromString(BaseItem->ItemName));
				ItemWidget->ItemAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"),Items[CurrentIndex].ItemAmount)));
				
				ItemWidget->OnClickWidget.AddDynamic(this, &ThisClass::OnClickItemWidget);
			}
			else if(SlotsCount >= CurrentIndex)
			{
				ItemWidget->SetItemState(CurrentInventoryTab == EHoldItem::EHI_Potion ? EInventoryItemState::EIIS_PotionEmpty : EInventoryItemState::EIIS_Empty);

				if (CurrentInventoryTab == EHoldItem::EHI_Potion) //craft potion options
				{
					ItemWidget->ItemImg->SetBrushResourceObject(Cast<UObject>(AddIconTexture));
					ItemWidget->OnClickWidget.AddDynamic(this, &ThisClass::OnClickItemWidget);
				}
				
			}
			else
			{
				ItemWidget->ItemImg->SetBrushResourceObject(Cast<UObject>(SlotLockedTexture));
				ItemWidget->SetItemState(EInventoryItemState::EIIS_Locked);
			}

			CurrentIndex++;
		}
		
	}

	//	TObjectPtr<UTexture> LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Images/Panels/InventoryItemBase"));
	// 0 1 2 
	// 3 4 5
	// 6 7 8

	//(2,1) (x,y)
	//Columns * x + y

}


void UBagUserWidget::OnClickItemWidget(const uint32 ItemId, EInventoryItemState CurrItemState)
{
	if (UWoDGameInstance* WoDGameInstance = GetWorld()->GetGameInstance<UWoDGameInstance>())
	{
		if (AWizard* OwningWizard = GetOwningPlayerPawn<AWizard>())
		{
			if (CurrItemState == EInventoryItemState::EIIS_Occupied)
			{
				WoDGameInstance->SetEquippedItem(CurrentInventoryTab, ItemId);
				OwningWizard->ChangeItemInventory(ItemId, CurrentInventoryTab);
			}
			else if (CurrItemState == EInventoryItemState::EIIS_Empty && CurrentInventoryTab == EHoldItem::EHI_Potion)
			{
				//open potions recepies, craft selected if have enough resources
			}
		}
	}
	
}