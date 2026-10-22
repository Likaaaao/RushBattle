// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class RUSHBATTLE_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMyAttributeSet();
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, Speed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxSpeed)
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, MaxSpeed)

	// no replicated
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, Ammo)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, MaxAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, Energy)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, MaxEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Money;
	ATTRIBUTE_ACCESSORS_BASIC(UMyAttributeSet, Money)

public:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth)const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
	}

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxHealth, OldMaxHealth);
	}

    UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldSpeed)const {
        GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Speed, OldSpeed);
    }

    UFUNCTION()
    void OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed)const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxSpeed, OldMaxSpeed);
    }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};
