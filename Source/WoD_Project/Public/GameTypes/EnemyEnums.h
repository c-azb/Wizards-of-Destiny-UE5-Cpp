#pragma once

UENUM()
enum class EEnemyState : uint8
{
	EES_None,

	EES_Patrolling, EES_Chasing, 
	
	EES_CloseAttack,EES_FarAttack,
	
	EES_GetHitFront, EES_GetHitBack, 
	
	EES_Death
};

UENUM()
enum EEnemyMovementState : uint8
{
	EES_Patrolling, EES_Chasing, EES_TakingCover, EES_Combat, EES_NoMovement
};


UENUM()
enum ETargetType : uint8
{
	ETT_None, ETT_Wizard, ETT_Spell, ETT_Monster
};