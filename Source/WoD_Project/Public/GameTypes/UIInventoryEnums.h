#pragma once

UENUM()
enum class EInventoryItemState : uint8
{
	EIIS_Occupied, EIIS_PotionEmpty, EIIS_Empty, EIIS_Locked
};

UENUM()
enum class EInventoryType : uint8
{
	EIT_Bag,EIT_Bank
};


