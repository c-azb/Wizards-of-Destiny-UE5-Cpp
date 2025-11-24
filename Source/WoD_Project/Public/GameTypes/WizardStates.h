#pragma once

UENUM()
enum class EWizardState : uint8
{
	EWCS_Rest, 
	EWCS_Combat, 
	EWCS_DoingCombo,

	/*********************/
	EWCS_ActivatingItemMIN,  

	EWCS_ThrowingSpell,	EWCS_HoldingSpell, EWCS_ThrowingPotion, EWCS_DrinkingPotion, EWCS_FlyingBroom,

	EWCS_ActivatingItemMAX,
	/*********************/
	EWCS_ChangeItem_MIN,

	EWCS_ChangeItem_Base, EWCS_ChangeItem_FlyingBroom,

	EWCS_ChangeItem_MAX,
	/*********************/

	EWCS_Falling,

	/*********************/

	EWCS_GettingHitFront,
	EWCS_GettingHitBack,

	/*********************/

	EWCS_Dead
};


UENUM()
enum class ESpellComboDir : uint8
{
	ESCD_Up = 'u', ESCD_Down = 'd', ESCD_Right = 'r', ESCD_Left = 'l'
};

UENUM()
enum class EHoldItem : uint8
{
	EHI_None, EHI_MagicWand, EHI_Potion, EHI_FlyingBroom, EHI_Grocery, EHI_Relics
};

UENUM()
enum class EActivateItem : uint8
{
	EAI_None, EAI_ThrowSpell, EAI_HoldSpell, EAI_ThrowPotion, EAI_DrinkPotion, EAI_FlyingBroom
};

UENUM()
enum class EWizardScore : uint8
{
	EWS_Monster_Kill, EWS_Wizard_Kill, EWS_SpellActivation, EWS_Pickup, EWS_Dead
};