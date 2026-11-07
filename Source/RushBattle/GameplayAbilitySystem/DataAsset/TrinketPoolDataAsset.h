// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrinketPoolDataAsset.generated.h"

/**
 * 
 */
class UTrinketDataAsset;

UCLASS()
class RUSHBATTLE_API UTrinketPoolDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Trinket")
	TArray<TObjectPtr<UTrinketDataAsset>> TrinketPool;

public:
	UFUNCTION(BlueprintCallable, Category = "Trinket")
	UTrinketDataAsset* RollTrinketOne()const;

	UFUNCTION(BlueprintCallable, Category = "Trinket")
	TArray<UTrinketDataAsset*> RollTrinketCount(int32 count)const;
};
