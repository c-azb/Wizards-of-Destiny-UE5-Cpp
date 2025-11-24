// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OverlapInteractionsComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WOD_PROJECT_API UOverlapInteractionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOverlapInteractionsComponent();
	void SetupOverlapEvent(UPrimitiveComponent* MeshOverlapDetection);
	
private:
	TObjectPtr<ABaseCharacter> BaseCharacter = nullptr;

	bool GetBaseCharacter();
	UFUNCTION() void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
