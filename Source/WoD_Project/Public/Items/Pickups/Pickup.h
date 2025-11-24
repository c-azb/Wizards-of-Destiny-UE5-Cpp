#pragma once

#include "GameTypes/PickUpEnums.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Pickup.generated.h"

class USphereComponent;
class UNiagaraSystem;

UCLASS()
class WOD_PROJECT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "Att_Pickups") uint32 PickUpID = 0;
	UPROPERTY(EditAnywhere, Category = "Att_Pickups") EPickUpType PickUpType = EPickUpType::EPUT_Grocery;

	void Pick(AActor* Picker, uint32& OutPickupId, EPickUpType& OutPickupType);

protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> PickupMesh;
	UPROPERTY(VisibleAnywhere) TObjectPtr<USphereComponent> PickupTrigger;
	UPROPERTY(EditAnywhere, Category = "Att_Pickups") TObjectPtr<UNiagaraSystem> OnPickVFX;

	float TimerCount = 0;
	FVector RestPosition;


};
