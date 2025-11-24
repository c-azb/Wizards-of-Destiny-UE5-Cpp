// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardWorldUtils/WorldEnemySpawner.h"
#include "Components/SphereComponent.h"
#include "GameTypes/ActorTagsDef.h"

AWorldEnemySpawner::AWorldEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject< USphereComponent>(FName("SpawnArea"));
	SetRootComponent(SpawnArea);
	SpawnArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SpawnArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
}

void AWorldEnemySpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CanSpawnNewEnemy())
	{
		if (SpawnDelay <= 0)
		{
			SpawnNewEnemy();
			SpawnDelayMult = FMath::RandRange(1, 10);
		}
		else
		{
			SpawnDelay -= DeltaSeconds * SpawnDelayMult;
		}
	}
}

void AWorldEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SpawnArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
		SpawnArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
	}
}

void AWorldEnemySpawner::DestroyAllEnemys()
{
	for (const auto& Enemy : EnemysInWorld)
	{
		RemoveEnemyFromWorld(Enemy);
	}
}

bool AWorldEnemySpawner::CanSpawnNewEnemy()
{
	return HasAuthority() && EnemysInWorld.Num() < MaxEnemysOnArea && TargetsInArea.Num() >= 1;
}

void AWorldEnemySpawner::SpawnNewEnemy()
{
	uint32 MaximunOptions = EnemysOptions.Num();
	if (MaximunOptions > 0)
	{
		if (TSubclassOf<AActor> Option = EnemysOptions[FMath::RandRange(1, MaximunOptions)])
		{
			float Radius = (GetMeanTargetPositions() - GetActorLocation()).Size2D();
			float RadiusDisplace = FMath::RandRange(0, 1000);

			Radius = FMath::Clamp(Radius + RadiusDisplace, 0, SpawnArea->GetUnscaledSphereRadius());
			FVector DisplaceDir = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0).GetSafeNormal();
			FVector EnemyLocation2D = GetActorLocation() + DisplaceDir * Radius;
			

			AActor* NewEnemy = GetWorld()->SpawnActor(Option);

			//setup position/rotation of enemy
			EnemysInWorld.Add(NewEnemy);
		}
	}
}

void AWorldEnemySpawner::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(WIZARD_TAG))
	{
		TargetsInArea.Add(OtherActor);
		if (TargetsInArea.Num() == 1)
		{	
			MaxEnemysOnArea = FMath::RandRange(MinRangeEnemys, MaxRangeEnemys);
		}
	}
}

void AWorldEnemySpawner::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(WIZARD_TAG))
	{
		TargetsInArea.Remove(OtherActor);
		if (TargetsInArea.Num() == 0)
		{
			DestroyAllEnemys();
			SpawnDelay=0;
		}
	}
}

FVector AWorldEnemySpawner::GetMeanTargetPositions()
{
	FVector MeanPos = FVector::ZeroVector;
	uint32 TargetsCount = TargetsInArea.Num();

	if (TargetsCount >= 1)
	{
		for (const auto& Target : TargetsInArea)
		{
			MeanPos += Target->GetActorLocation();
		}
		MeanPos /= TargetsCount;
	}

	return MeanPos;
}

void AWorldEnemySpawner::RemoveEnemyFromWorld(TObjectPtr<AActor> EnemyToRemove)
{
	//call after enemy death delay
	EnemysInWorld.Remove(EnemyToRemove);
	EnemyToRemove->Destroy();
}

