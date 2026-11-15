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

	UPROPERTY(BlueprintReadOnly, Category = "Run")
	EWaveState WaveState= EWaveState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
    int32 CurrentWaveIndex = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
	int32 TotalWaves = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Run")
	int32 EnemiesAlive = 0;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveStateChanged OnWaveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnRunEnded OnRunEnded;
protected:
    virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartRun();

    UFUNCTION(BlueprintCallable, Category = "Wave")
	void EnterState(EWaveState NewState);
	

};
