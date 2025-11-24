// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldEnemySpawner.generated.h"

class USphereComponent;

UCLASS()
class WOD_PROJECT_API AWorldEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldEnemySpawner();
	virtual void Tick(float DeltaSeconds) override;
	void RemoveEnemyFromWorld(TObjectPtr<AActor> EnemyToRemove);

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<USphereComponent> SpawnArea;

	UPROPERTY(EditAnywhere, Category = "Att_EnemySpawner") TArray<TSubclassOf<AActor>> EnemysOptions;

	UPROPERTY(EditAnywhere, Category = "Att_EnemySpawner") int32 MaxRangeEnemys=10;
	UPROPERTY(EditAnywhere, Category = "Att_EnemySpawner") int32 MinRangeEnemys=5;
	float SpawnDelay = 0;
	float SpawnDelayMult = 1;

	TArray<AActor*> TargetsInArea;

	int32 MaxEnemysOnArea = 0;
	TArray< TObjectPtr<AActor> > EnemysInWorld;

	void DestroyAllEnemys();

	bool CanSpawnNewEnemy();
	void SpawnNewEnemy();
	FVector GetMeanTargetPositions();

	UFUNCTION() void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
