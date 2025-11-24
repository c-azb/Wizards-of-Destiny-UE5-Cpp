

#include "Items/ApplyDamageComponent.h"
#include "Engine/DamageEvents.h"
#include "GameTypes/ActorTagsDef.h"

UApplyDamageComponent::UApplyDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UApplyDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	DamageInstigator = GetOwner()->GetInstigatorController();
}

bool UApplyDamageComponent::MakeDamage(AActor* OtherActor, const FHitResult& HitResult, const float DamageMult)
{
	//if (GetOwner()->HasAuthority() && CanApplyDamage(OtherActor))
	if (CanApplyDamage(OtherActor))
	{
		if (ApplyRadialDamage)
		{
			MakeRadialDamage(OtherActor, HitResult, DamageMult);
		}
		else
		{
			MakePointDamage(OtherActor, HitResult, DamageMult);
		}
	}
	return CanApplyDamage(OtherActor);
}

void UApplyDamageComponent::MakePointDamage(AActor* OtherActor, const FHitResult& HitResult, const float DamageMult)
{
	FPointDamageEvent PointDamageEvent(Damage, HitResult, HitResult.ImpactNormal, nullptr);
	OtherActor->TakeDamage(Damage * DamageMult, PointDamageEvent, DamageInstigator, GetOwner());
}

void UApplyDamageComponent::MakeRadialDamage(AActor* OtherActor, const FHitResult& HitResult,const float DamageMult)
{
	FRadialDamageParams RadialDamageParams(Damage, MinimunDamage, InnerRadius, OutterRadius, DamageFalloff);
	FRadialDamageEvent RadialDamageEvent;
	RadialDamageEvent.Params = RadialDamageParams;
	RadialDamageEvent.Origin = GetOwner()->GetActorLocation();
	RadialDamageEvent.ComponentHits = TArray<FHitResult>{ HitResult };
	
	OtherActor->TakeDamage(Damage * DamageMult, RadialDamageEvent, DamageInstigator, GetOwner());
}

bool UApplyDamageComponent::CanApplyDamage(AActor* OtherActor)
{
	return (OtherActor->ActorHasTag(SPELL_TAG) && DamageSpells) || 
		(OtherActor->ActorHasTag(WIZARD_TAG) && DamageCharacters) || 
		(OtherActor->ActorHasTag(MONSTER_TAG) && DamageCharacters);
}



