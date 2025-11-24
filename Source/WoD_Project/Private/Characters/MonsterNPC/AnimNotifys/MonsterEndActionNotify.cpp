// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MonsterNPC/AnimNotifys/MonsterEndActionNotify.h"
#include "Characters/MonsterNPC/EnemyNPC.h"

void UMonsterEndActionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AEnemyNPC* EnemyNPC = Cast<AEnemyNPC>(MeshComp->GetOwner()))
	{
		if (bEndHitHard)
		{
			EnemyNPC->EndGetHitHard();
		}
	}
}
