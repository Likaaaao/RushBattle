// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UMyAttributeSet::UMyAttributeSet() {
	Health = 100.0f;
    MaxHealth = 100.0f;
    Speed = 600.0f;
    MaxSpeed = 600.0f;
}
void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);//ÍøÂçÍ¬²½
    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Speed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxSpeed, COND_None, REPNOTIFY_Always);
    }


void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
    Super::PreAttributeChange(Attribute, NewValue);
    if (Attribute == GetHealthAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }
    else if (Attribute == GetMaxHealthAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if(Attribute == GetSpeedAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSpeed());
    }
    else if (Attribute == GetMaxSpeedAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetAmmoAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmo());
    }
    else if (Attribute == GetMaxAmmoAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetEnergyAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
    }
    else if (Attribute == GetMaxEnergyAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }
    else if (Attribute == GetMoneyAttribute()) {
        NewValue = FMath::Max(NewValue, 0.f);
    }

}

//replicate
void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetAmmoAttribute())
    {
        SetAmmo(FMath::Clamp(GetAmmo(), 0.f, GetMaxAmmo()));
    }
    else if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
    {
        SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
    }

}


