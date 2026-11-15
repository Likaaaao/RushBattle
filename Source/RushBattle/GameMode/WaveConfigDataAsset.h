// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RushBattle/GameMode/WaveTypes.h"
#include "WaveConfigDataAsset.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class RUSHBATTLE_API UWaveConfigDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveInterval = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FWaveDefinition> Waves;
	
};
