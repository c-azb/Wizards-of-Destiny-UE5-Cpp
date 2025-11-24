// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/AnimNotifys/EnemyAtkNotify.h"
#include "Characters/MonsterNPC/EnemyNPC.h"

void UEnemyAtkNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AEnemyNPC* EnemyNPC = Cast<AEnemyNPC>(MeshComp->GetOwner()))
	{
		if (bPreparation)
		{
			EnemyNPC->SetupAttack(bJumpAtk);
		}
		else
		{
			if (bTriggerAtk)
			{
				EnemyNPC->Attack(DamageMult, DamageRangeMult);
			}
			else
			{
				EnemyNPC->EndAttack();
			}
		}
	}

}
