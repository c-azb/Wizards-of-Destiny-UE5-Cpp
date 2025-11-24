// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/FlyingBroom.h"
#include "NiagaraComponent.h"

AFlyingBroom::AFlyingBroom()
{
    FBNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraComponent"));
    FBNiagaraComponent->SetupAttachment(ItemMesh);
}

void AFlyingBroom::BeginPlay()
{
    Super::BeginPlay();
    if (EquipFBMontage)
    {
        FName SecName = FName(FString::Printf(TEXT("Sec%d"), FMath::RandRange(1, 3)));
        if (UAnimInstance* AnimInstance = ItemMesh->GetAnimInstance())
        {
            AnimInstance->Montage_Play(EquipFBMontage);
            AnimInstance->Montage_JumpToSection(SecName, EquipFBMontage);
        }
    }
}

EHoldItem AFlyingBroom::GetItemType()
{
    return EHoldItem::EHI_FlyingBroom;
}

FResourcesStruct AFlyingBroom::GetItemCost(const bool bEquip)
{
    if (bEquip)
    {
        return EquipItemCost;
    }
    return FResourcesStruct();
}

bool AFlyingBroom::GetContinuousItemCost(FResourcesStruct& ContinuousCost)
{
    ContinuousCost.Stamina = StaminaConsume;
    ContinuousCost.Special = 0;;
    ContinuousCost.Mana = 0;;
    return true;
}



