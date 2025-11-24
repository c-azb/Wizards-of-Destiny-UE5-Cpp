#pragma once

#include "Engine/DataTable.h"
#include "GameTypes/PickUpEnums.h"
#include "BaseItemStruct.generated.h"


USTRUCT(BlueprintType)
struct FBaseItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UTexture2D> ItemImg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) UClass* ItemClass;
};

USTRUCT(BlueprintType)
struct FMWData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	//row name is equal to the code
};

USTRUCT(BlueprintType)
struct FSpellData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int SpellId;
	//row name is equal to the spell combination
};

USTRUCT(BlueprintType)
struct FPotionData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	//row name is equal to the code

	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UTexture2D> RecipeImg;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<EGroceryType> GrocerysRequests;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<EGroceryValue> GrocerysRequestsValues;

};

USTRUCT(BlueprintType)
struct FFBData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	//row name is equal to the code
};


USTRUCT(BlueprintType)
struct FCharacterData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	//row name is equal to the code
};


USTRUCT(BlueprintType)
struct FGroceryData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EGroceryType GroceryType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EGroceryValue GroceryValue;

	//row name is equal to the code
};

USTRUCT(BlueprintType)
struct FRelicData : public FBaseItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 RelicValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) ERelicType RelicType;
	//row name is equal to the code
};





