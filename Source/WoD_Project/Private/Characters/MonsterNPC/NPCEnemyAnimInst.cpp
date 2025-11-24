// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/NPCEnemyAnimInst.h"
#include "Characters/MonsterNPC/EnemyNPC.h"
#include "GameFramework/CharacterMovementComponent.h"

void UNPCEnemyAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	EnemyNPC = Cast<AEnemyNPC>(TryGetPawnOwner());
}

void UNPCEnemyAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (EnemyNPC)
	{
		EEnemyState EnemyState = EnemyNPC->EnemyState;
		bOnAir = EnemyNPC->GetCharacterMovement()->IsFalling();
		MoveSpeed = EnemyNPC->GetVelocity().Size2D() / EnemyNPC->GetMaximunSpeed();
		bProvoked = EnemyState >= EEnemyState::EES_Chasing;
	}
}
