// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Spells/Spell.h"
#include "Items/HealthComponent.h"
#include "GameTypes/ActorTagsDef.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"	
#include "Items/MagicWand.h"

#include "Net/UnrealNetwork.h"

ASpell::ASpell()
{
	PrimaryActorTick.bCanEverTick = true;

	SpellHealth = CreateDefaultSubobject<UHealthComponent>(FName("SpellHealth"));
	SpellHealth->SetIsReplicated(true);

	ApplyDamageComponent = CreateDefaultSubobject<UApplyDamageComponent>(FName("ApplyDamage"));
	ApplyDamageComponent->SetIsReplicated(true);

	bReplicates = true;

	Tags.Add(SPELL_TAG);
}

void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpawnHitVFXCounter > 0)
	{
		SpawnHitVFXCounter = FMath::Clamp(SpawnHitVFXCounter - DeltaTime, 0, 1);
	}
}

void ASpell::BeginPlay()
{
	Super::BeginPlay();
	SpellHealth->InicializeResource();
	MagicWandRef = Cast<AMagicWand>(GetOwner());

	if (MagicWandRef)
	{
		SpawnSpellVFX(CastSpellVFX, MagicWandRef->WandPoint);
	}

	if (SpellSound)
	{
		SpellAudioComponent = UGameplayStatics::SpawnSoundAttached(
			SpellSound,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			EAttachLocation::KeepWorldPosition,
			true);
	}

}

void ASpell::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpell, IsDestroying);
}


float ASpell::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!SpellHealth->IsAlive()) return 0.0f;

	float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SpellHealth->GetDamage(DamageTaken);

	if (!SpellHealth->IsAlive())
	{
		SpellDeath();
	}

	return DamageTaken;
}

void ASpell::StopSpell(){}

void ASpell::SpellDeath()
{
	if (IsDestroying) return;

	if (HasAuthority())
	{
		IsDestroying = true;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ThisClass::DestroySpell, DestroyDelay);
	}

	PlaySpellDeathCosmetics();
}

void ASpell::PlaySpellDeathCosmetics()
{
	if (SpellAudioComponent)
	{
		SpellAudioComponent->SetTriggerParameter(FName("EndTrigger"));
	}
	SpawnSpellVFX(DeathSpellVFX);
}


void ASpell::DestroySpell()
{
	Destroy();
}



FVector ASpell::GetWandPointLoc()
{
	if (MagicWandRef)
	{
		return MagicWandRef->WandPoint->GetComponentLocation();
	}
	return FVector();
}

bool ASpell::GetMWAimTargetPos(const float CurrentReach,FHitResult* OutHitResult,FRotator* OutRot,FVector* OutPos)
{
	return MagicWandRef->GetAimTargetPos(MaxSpellDistance, CurrentReach, OutHitResult, OutRot, OutPos);
}

void ASpell::SpawnSpellVFX(TObjectPtr<UNiagaraSystem> SpellVFX,const TObjectPtr<USceneComponent> AttachTo, const FHitResult* HitInfo,const bool AttachKeepingPos)
{
	if (SpellVFX)
	{
		//UNiagaraComponent* NiagaraComponent;

		if (AttachTo && !AttachKeepingPos)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(SpellVFX, AttachTo, FName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
		}
		else
		{
			FRotator Rotation = HitInfo == nullptr ? GetActorRotation() : HitInfo->ImpactNormal.Rotation();
			FVector SpawnLocation = HitInfo == nullptr ? GetActorLocation() : HitInfo->ImpactPoint + HitInfo->ImpactNormal*3;
			
			if (AttachTo && AttachKeepingPos)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(SpellVFX, AttachTo, FName(), SpawnLocation, Rotation, EAttachLocation::Type::KeepWorldPosition, true);
			}
			else
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SpellVFX, SpawnLocation, Rotation);
			}
		}		
	}
}

void ASpell::OnRepIsDestroying()
{
	if (IsDestroying)
	{
		PlaySpellDeathCosmetics();
	}
}