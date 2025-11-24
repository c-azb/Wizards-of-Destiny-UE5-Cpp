// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TriggerEffect/DamageTriggerEffect.h"
#include "NiagaraComponent.h"

#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Items/ApplyDamageComponent.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

ADamageTriggerEffect::ADamageTriggerEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	EffectNiagara = CreateDefaultSubobject<UNiagaraComponent>(FName("EffectNiagara"));
	SetRootComponent(EffectNiagara);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(FName("SphereCollider"));
	SphereCollider->SetupAttachment(GetRootComponent());

	SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	SphereCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Ignore);
	SphereCollider->SetSphereRadius(300);

	ApplyDamageComponent = CreateDefaultSubobject<UApplyDamageComponent>(FName("ApplyDamage"));

	bReplicates = true;
}

void ADamageTriggerEffect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bApplyTickDamage && !bUseNiagaraCollision)
	{
		TArray<AActor*> OverlappingActors;
		SphereCollider->GetOverlappingActors(OverlappingActors);

		for (const auto& Actor : OverlappingActors)
		{
			ApplyDamageComponent->MakeDamage(Actor, FHitResult(), DeltaSeconds);
		}
	}
}

void ADamageTriggerEffect::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	if (!bUseNiagaraCollision) return;
	for (int32 i = 0; i < Data.Num(); i ++)
	{
		FVector ParticlePos = Data[i].Position + GetActorUpVector() * NiagaraCollSize.Z;
		FQuat BoxRot = FQuat(GetActorRotation());
		FCollisionShape CollisionShape = FCollisionShape::MakeBox(NiagaraCollSize);
		FCollisionQueryParams CollisionQuerry;
		CollisionQuerry.AddIgnoredActor(this);

		TArray<FOverlapResult> OutActors;

		DrawDebugBox(GetWorld(), ParticlePos, FVector(NiagaraCollSize), BoxRot, FColor::Red, false);
		GetWorld()->OverlapMultiByChannel(OutActors, ParticlePos, BoxRot, ECollisionChannel::ECC_WorldDynamic, CollisionShape, CollisionQuerry);

		for (const auto& OverlapResult : OutActors)
		{
			//ApplyDamageComponent->MakeDamage(OverlapResult.GetActor(), FHitResult(), GetWorld()->DeltaTimeSeconds);
			ApplyDamageComponent->MakeDamage(OverlapResult.GetActor(), FHitResult(), 1); //using delay on PS send data
		}
	}

}

void ADamageTriggerEffect::BeginPlay()
{
	Super::BeginPlay();

	if (EffectSoundSFX)
	{
		AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, EffectSoundSFX, GetActorLocation());
	}
	if (bUseNiagaraCollision)
	{
		EffectNiagara->SetVariableObject(FName("ExportCallback"), this);
	}
	
	EffectNiagara->OnSystemFinished.AddDynamic(this, &ThisClass::OnEffectFinished);
}


void ADamageTriggerEffect::OnEffectFinished(UNiagaraComponent* PSystem)
{
	if (AudioComponent)
	{
		AudioComponent->SetTriggerParameter(FName("EndTrigger"));
	}

	Destroy();
}
