// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveTypes.generated.h"

class ARushBattleEnemyCharacter;
class UCharacterInitStats;
class AActor;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FWaveSpawnEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TSubclassOf<ARushBattleEnemyCharacter> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TObjectPtr<UCharacterInitStats> InitStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    FName SpawnPointTag = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 SpawnCount=1;
};

USTRUCT(BlueprintType)
struct FWaveDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FWaveSpawnEntry> Spawns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float SpawnDelay=3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float SpawnInterval = 0.4f;

};

UENUM(BlueprintType)
enum class EWaveState: uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Preparing   UMETA(DisplayName = "Preparing"),
    Spawning    UMETA(DisplayName = "Spawning"),
    Fighting    UMETA(DisplayName = "Fighting"),
    WaveCleared UMETA(DisplayName = "WaveCleared"),
    RunEnded    UMETA(DisplayName = "RunEnded")
};




