// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
UResourceSet::UResourceSet()
{ 
    DamageMultiplier = 1.f;
    CritChanceBonus = 0.f;
    CritMultiplierBonus=1.f;
    FireRateMultiplier = 1.f;
    ReloadSpeedMultiplier = 1.0f;
}

void UResourceSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetAmmoAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmo());
    }
    else if (Attribute == GetMaxAmmoAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetEnergyAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
    }
    else if (Attribute == GetMaxEnergyAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetMoneyAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetHealthPointAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealthPoint());
    }

    else if (Attribute == GetMaxHealthPointAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.f);
    }

    else if (Attribute == GetAmmoPointAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmoPoint());
    }
    else if (Attribute == GetMaxAmmoPointAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.f);
    }

    else if (Attribute == GetEnergyPointAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergyPoint());
    }
    else if (Attribute == GetMaxEnergyPointAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetDamageMultiplierAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetFireRateMultiplierAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetCritChanceBonusAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
    }
    else if (Attribute == GetCritMultiplierBonusAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetReloadSpeedMultiplierAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
}

void UResourceSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetAmmoAttribute())
    {
        SetAmmo(FMath::Clamp(GetAmmo(), 0.f, GetMaxAmmo()));
    }
    else if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
    {
        SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
    }
    else if (Data.EvaluatedData.Attribute == GetHealthPointAttribute())
    {
        SetHealthPoint(FMath::Clamp(GetHealthPoint(), 0.f, GetMaxHealthPoint()));
    }
    else if (Data.EvaluatedData.Attribute == GetAmmoPointAttribute())
    {
        SetAmmoPoint(FMath::Clamp(GetAmmoPoint(), 0.f, GetMaxAmmoPoint()));
    }
    else if (Data.EvaluatedData.Attribute == GetEnergyPointAttribute())
    {
        SetEnergyPoint(FMath::Clamp(GetEnergyPoint(), 0.f, GetMaxEnergyPoint()));
    }


}

