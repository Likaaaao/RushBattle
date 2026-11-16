// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RushBattle/GameMode/WaveTypes.h"
#include "RushBattle/GameMode/WaveConfigDataAsset.h"
#include "RushBattleGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveStateChanged, EWaveState, NewState, int32, WaveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRunEnded, bool, bVictory);

UCLASS()
class RUSHBATTLE_API ARushBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TObjectPtr<UWaveConfigDataAsset> WaveConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run|Spawn")
	float SpawnRadiusMin=1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Run|Spawn")
	float SpawnRadiusMax= 2500.f;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
	EWaveState WaveState= EWaveState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 CurrentWaveIndex = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
	int32 TotalWaves = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
	int32 EnemiesAlive = 0;

	//生成队列
	UPROPERTY(BlueprintReadOnly, Category = "Run|Spawn")
	TArray<int32> PendingSpawns;	
	//生成下一个的下标
	UPROPERTY(BlueprintReadOnly, Category = "Run|Spawn")
	int32 SpawnCursor = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Run|Timer")
	FTimerHandle WaveTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Run|Timer")
	FTimerHandle SpawnTimerHandle;


	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveStateChanged OnWaveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnRunEnded OnRunEnded;
protected:
	TArray<TWeakObjectPtr<ARushBattleEnemyCharacter>> SpawnedEnemies;

	UPROPERTY(BlueprintReadOnly, Category = "Run|Timer")
	FTimerHandle CheckClearTimerHandle;

	void CheckWaveCleared();

	void StartNextWave();

	void OnWaveCleared();

protected:
    virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartRun();

    UFUNCTION(BlueprintCallable, Category = "Wave")
	void EnterState(EWaveState NewState);
	
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave(int32 WaveIndex);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void BeginSpawning();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	FVector ResolveSpawnLocation(const FWaveSpawnEntry& Entry) const;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SpawnOneEnemy();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void OnWaveAllSpawned();


};
