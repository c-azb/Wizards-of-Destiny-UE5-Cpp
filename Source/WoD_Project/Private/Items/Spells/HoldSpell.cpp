// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Spells/HoldSpell.h"
#include "NiagaraComponent.h"
//#include "Items/MagicWand.h"
#include "Net/UnrealNetwork.h"


AHoldSpell::AHoldSpell()
{
	SpellNiagara = CreateDefaultSubobject<UNiagaraComponent>(FName("SpellNiagara"));
	SetRootComponent(SpellNiagara);

	SpellMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SpellMesh"));
	SpellMesh->SetupAttachment(GetRootComponent());
	SpellMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//SpellMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);


	ActivateItemType = EActivateItem::EAI_HoldSpell;
}

void AHoldSpell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (MaxSpellDistance == 0 && GetInstigator())
	{
		SetActorLocation(GetInstigator()->GetActorLocation());
	}
	else
	{
		SetActorLocation(GetWandPointLoc());
		ProcessHoldSpell(DeltaSeconds);
	}
}

void AHoldSpell::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHoldSpell, CurrentServerEndPoint);
}

void AHoldSpell::BeginPlay()
{
	Super::BeginPlay();

	CurrentLocalEndPoint = GetActorLocation();
	SetBeamParameters(GetActorRotation());
}

void AHoldSpell::ProcessHoldSpell(const float DeltaSeconds)
{
	if (HasAuthority())
	{
		CurrentDistance = FMath::Clamp(CurrentDistance + DeltaSeconds * GrowSpeed, 0, MaxSpellDistance);
		FHitResult TargetHit;

		if (GetMWAimTargetPos(CurrentDistance, &TargetHit, nullptr, &CurrentServerEndPoint))
		{
			CurrentDistance = TargetHit.Distance;

			const bool AttachTo = ApplyDamageComponent->MakeDamage(TargetHit.GetActor(), TargetHit,DeltaSeconds * 1);
			
			if (SpawnHitVFXCounter <= .0001f || (CurrentLocalEndPoint-CurrentServerEndPoint).SquaredLength()>=50)
			{
				SpawnHitVFXCounter = .5f;
				SpawnSpellVFX(HitSpellVFX, AttachTo ? TargetHit.GetActor()->GetRootComponent() : nullptr, &TargetHit, AttachTo);
			}
		}
	}

	CurrentLocalEndPoint = FMath::Lerp<FVector, float>(CurrentLocalEndPoint, CurrentServerEndPoint, DeltaSeconds * 40);
	FRotator SpellRot = (CurrentLocalEndPoint - GetWandPointLoc()).Rotation();
	SetBeamParameters(SpellRot);

	if (SpellMesh->GetStaticMesh())
	{
		const FVector SpellMeshLocation = CurrentLocalEndPoint - SpellRot.Vector() * MinOffsetFromWall;
		SpellMesh->SetWorldLocation(SpellMeshLocation);
		//FHitResult HitResult;
		//SpellMesh->SetWorldLocation(SpellMeshLocation,true,&HitResult);
		/*if (HitResult.GetActor())
		{SpellMesh->SetWorldRotation(HitResult.ImpactNormal.Rotation(), true);
		}
		else
		{
			SpellMesh->SetWorldRotation(SpellRot,true);
		}*/
	}
}

void AHoldSpell::SetBeamParameters(const FRotator& SpellRot)
{
	//DrawDebugLine(GetWorld(), GetWandPointLoc(), CurrentSpellEndPoint, FColor::Red);
	SpellNiagara->SetVectorParameter(FName(TEXT("BeamEnd")), CurrentLocalEndPoint);
	SpellNiagara->SetFloatParameter(FName(TEXT("Velocity_")), CurrentDistance);
	SetActorRotation(SpellRot);
}


