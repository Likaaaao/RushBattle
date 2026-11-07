// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTableDataAsset.generated.h"

class UTrinketPoolDataAsset;
class ATrinketPickup;
/**
 * 
 */
UCLASS()
class RUSHBATTLE_API ULootTableDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTrinketPoolDataAsset> TrinketPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinCount=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount=2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
	TSubclassOf<ATrinketPickup> PickupClass;      
};
