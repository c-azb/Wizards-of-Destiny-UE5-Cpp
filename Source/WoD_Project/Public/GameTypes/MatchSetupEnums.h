#pragma once

UENUM()
enum class EPlayOption : uint8
{
	EPO_None, EPO_WizardDuel, EPO_WizardWar, EPO_Chalange
};

UENUM()
enum class EGameModeType : uint8
{
	EGMT_None, EGMT_1v1, EGMT_2v2, EGMT_FFA, EGMT_WizardWar, EGMT_Chalange
};

UENUM()
enum class ENetworkType : uint8
{
	ENT_None, ENT_Offline, ENT_Online, ENT_Online_Ranked
};

UENUM()
enum class EMapId : uint8
{
	EMI_Castle, EMI_Dungeon, EMI_GreenFlorest, EMI_Ruins, EMI_None
};