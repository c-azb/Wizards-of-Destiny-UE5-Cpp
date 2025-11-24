#pragma once

//USTRUCT()
struct FItemInventory
{
	//GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere) uint32 ItemCode = 0;
	UPROPERTY(VisibleAnywhere) uint32 UsageUpgrade = 0;
	UPROPERTY(VisibleAnywhere) uint32 Level = 0;

	FItemInventory(uint32 ItemCode_) { ItemCode = ItemCode_; UsageUpgrade = 0; Level = 0; }
};