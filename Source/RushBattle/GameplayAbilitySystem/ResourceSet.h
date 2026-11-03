// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ResourceSet.generated.h"

/**
 * 
 */
UCLASS()
class RUSHBATTLE_API UResourceSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UResourceSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, Ammo)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, MaxAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, Energy)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, MaxEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Money;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, Money)


	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ResourcePoint")
	FGameplayAttributeData HealthPoint;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, HealthPoint)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ResourcePoint")
	FGameplayAttributeData MaxHealthPoint;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, MaxHealthPoint)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ResourcePoint")
	FGameplayAttributeData AmmoPoint;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, AmmoPoint)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ResourcePoint")
	FGameplayAttributeData MaxAmmoPoint;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, MaxAmmoPoint)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ResourcePoint")
	FGameplayAttributeData EnergyPoint;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, EnergyPoint)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ResourcePoint")
	FGameplayAttributeData MaxEnergyPoint;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, MaxEnergyPoint)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|WeaponData")
	FGameplayAttributeData DamageMultiplier;
    ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, DamageMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|WeaponData")
	FGameplayAttributeData FireRateMultiplier;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, FireRateMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|WeaponData")
	FGameplayAttributeData CritChanceBonus;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, CritChanceBonus)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|WeaponData")
	FGameplayAttributeData CritMultiplierBonus;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, CritMultiplierBonus)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|WeaponData")
	FGameplayAttributeData ReloadSpeedMultiplier;
	ATTRIBUTE_ACCESSORS_BASIC(UResourceSet, ReloadSpeedMultiplier)

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
