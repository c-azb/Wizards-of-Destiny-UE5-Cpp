// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potions/ThrowPotion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameTypes/ActorTagsDef.h"


void AThrowPotion::Destroyed()
{
	Super::Destroyed();
	if (ItemMesh->IsSimulatingPhysics())
	{
		UGameplayStatics::PlaySoundAtLocation(this, PotionHitSFX, ItemMesh->GetComponentLocation());
	}
}

//needs to enable "notify hit envents" on BP....
bool AThrowPotion::ActivateItem()
{
	AController* OwnerController = GetInstigatorController();
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	Location = Location + Rotation.Vector() * FVector::Dist2D(GetOwner()->GetActorLocation(), Location);

	FHitResult HitResult;
	FVector EndPoint = Location + Rotation.Vector() * 100000;
	FCollisionQueryParams CollQuery;
	CollQuery.AddIgnoredActor(this);
	CollQuery.AddIgnoredActor(GetOwner());
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Location, EndPoint, ECollisionChannel::ECC_Visibility, CollQuery);
	
	const float MaxForce = 300000;
	EndPoint = bHit ? HitResult.ImpactPoint : EndPoint;
	FVector ApplyForce = ((EndPoint - ItemMesh->GetComponentLocation()).GetSafeNormal()) * MaxForce;

	//set potion throw force by mouse hold button
	MulticastAddPotionForce(ApplyForce, GetActorLocation());

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::TriggerPotion, Rotation);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 5, false);

	ItemMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnPotionHit);

	return true;
}

void AThrowPotion::MulticastAddPotionForce_Implementation(const FVector_NetQuantize ThrowForce, const FVector_NetQuantize StartPos)
{
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ItemMesh->SetSimulatePhysics(true);

	SetActorLocation(StartPos);
	ItemMesh->AddForce(ThrowForce);
	UGameplayStatics::PlaySoundAtLocation(this, ThrowPotionSFX, ItemMesh->GetComponentLocation());
}

EActivateItem AThrowPotion::GetActivateType()
{
	return EActivateItem::EAI_ThrowPotion;
}


void AThrowPotion::OnPotionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const bool bHitInteractiveActor = OtherActor->ActorHasTag(WIZARD_TAG) || OtherActor->ActorHasTag(MONSTER_TAG) || OtherActor->ActorHasTag(SPELL_TAG);

	if (bHitInteractiveActor && !bTriggerOnHitPawn) return;

	TriggerPotion(UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal));
}

void AThrowPotion::TriggerPotion(const FRotator ImpactNormalRot)
{
	if (PotionTriggerEffect)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = SpawnParameters.Instigator = GetInstigator();
		FVector EffectLoc = ItemMesh->GetComponentLocation() + ImpactNormalRot.Vector() * 5;
		GetWorld()->SpawnActor<AActor>(PotionTriggerEffect, EffectLoc, ImpactNormalRot, SpawnParameters);
	}
	Destroy();
}



