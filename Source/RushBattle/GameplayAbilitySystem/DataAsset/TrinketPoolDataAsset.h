// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrinketPoolDataAsset.generated.h"

/**
 * 
 */
class UTinketDataAsset;

UCLASS()
class RUSHBATTLE_API UTrinketPoolDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Trinket")
	TArray<TSubclassOf<UTinketDataAsset>> TrinketPool;
};
