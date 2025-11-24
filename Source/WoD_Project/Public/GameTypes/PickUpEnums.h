#pragma once

UENUM()
enum class EPickUpType : uint8
{
	EPUT_Grocery,EPUT_Relic
};

UENUM()
enum class EGroceryType : uint8
{
	EGT_Bone, EGT_Meat, 
	EGT_Miracle, EGT_Ancient, 
	EGT_Poisoness, EGT_Unstable
};

UENUM()
enum class EGroceryValue : uint32
{
	EGV_Half, EGV_Full
};

UENUM()
enum class ERelicType : uint32
{
	ERT_Common, ERT_Rare, ERT_SuperRare
};