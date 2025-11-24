// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Spells/ThrowSpell.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"

AThrowSpell::AThrowSpell()
{
	SpellMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SpellMesh"));
	SetRootComponent(SpellMesh);

	SpellMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	SpellNiagara = CreateDefaultSubobject<UNiagaraComponent>(FName("SpellNiagara"));
	SpellNiagara->SetupAttachment(GetRootComponent());

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileComponent"));
	ProjectileComponent->SetIsReplicated(true);
	ProjectileComponent->InitialSpeed = 5000;
	ProjectileComponent->MaxSpeed = 5000;
	ProjectileComponent->ProjectileGravityScale = 0;

	ActivateItemType = EActivateItem::EAI_ThrowSpell;
}

void AThrowSpell::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AThrowSpell,FinalSpellPos);
}

void AThrowSpell::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();

	if (SpellMesh->GetStaticMesh())
	{
		SpellMesh->IgnoreActorWhenMoving(GetOwner(), true);
		if (AutoDestroyOnHit)
		{
			SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
		SpellMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHitHandle);
	}

	if (!AutoDestroyOnHit && HasAuthority())
	{
		FTimerHandle SpellLifeTimeHandle;
		GetWorldTimerManager().SetTimer(SpellLifeTimeHandle, this, &ThisClass::SpellDeath, SpellLifeTime);
	}
}

void AThrowSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && ProjectileComponent->bSimulationEnabled)
	{
		if ((GetActorLocation() - StartPosition).SquaredLength() >= MaxSpellDistance * MaxSpellDistance)
		{
			StopSpell();
		}
	}

	CheckSelfOverlapping();
}



void AThrowSpell::CheckSelfOverlapping()
{
	if (!SpellMesh->GetStaticMesh()) return;

	if (SpellMesh->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic) == ECollisionResponse::ECR_Overlap)
	{
		TArray<FOverlapInfo> OvelapsInfo;
		if (!SpellMesh->GetOverlapsWithActor(GetInstigator(), OvelapsInfo) || GetInstigator() == nullptr)
		{
			SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
			if (!AutoDestroyOnHit)
			{
				SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
			}
		}
	}
}

void AThrowSpell::OnHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsDestroying) return;

	bool AttachVFX = false;
	if (OtherActor != GetInstigator())
	{
		AttachVFX = ApplyDamageComponent->MakeDamage(OtherActor, Hit, 1);
	}

	StopSpell();

	if (SpawnHitVFXCounter <= .0001f)
	{
		SpawnHitVFXCounter = 1;
		SpawnSpellVFX(HitSpellVFX, AttachVFX ? OtherActor->GetRootComponent() : nullptr, &Hit, AttachVFX);
	}
}

void AThrowSpell::StopSpell()
{
	if (ProjectileComponent->bSimulationEnabled)
	{
		ProjectileComponent->bSimulationEnabled = false;
		ProjectileComponent->Velocity = FVector(0, 0, 0);

		if (AutoDestroyOnHit)
		{
			SpellMesh->SetHiddenInGame(true);
			SpellDeath();
		}
		if (HasAuthority())
		{
			FinalSpellPos = GetActorLocation();
		}
	}
}

void AThrowSpell::SpellDeath()
{
	if (!IsDestroying)
	{
		SpellMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Super::SpellDeath();
		StopSpell();
	}
}

void AThrowSpell::OnRepFinalPos()
{
	StopSpell();
	SetActorLocation(FinalSpellPos);
}

void AThrowSpell::OnRepIsDestroying()
{
	if(IsDestroying) SpellMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Super::OnRepIsDestroying();
}
