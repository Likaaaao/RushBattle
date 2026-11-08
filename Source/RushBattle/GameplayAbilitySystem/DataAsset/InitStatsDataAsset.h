#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InitStatsDataAsset.generated.h"

UCLASS(BlueprintType)
class RUSHBATTLE_API UCharacterInitStats : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float Speed = 600.f;
};

UCLASS(BlueprintType)
class RUSHBATTLE_API UPlayerInitStats : public UCharacterInitStats
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Ammo")
	float Ammo = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Ammo")
	float MaxAmmo = 120.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Energy")
	float Energy = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Energy")
	float MaxEnergy = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Money")
	float Money = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Points")
	float HealthPoint = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Points")
	float MaxHealthPoint = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Points")
	float AmmoPoint = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Points")
	float MaxAmmoPoint = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Points")
	float EnergyPoint = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource|Points")
	float MaxEnergyPoint = 3.f;
};