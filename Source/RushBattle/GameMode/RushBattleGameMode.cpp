// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattleGameMode.h"
namespace
{
	const TCHAR* WaveStateToString(EWaveState State)
	{
		switch (State)
		{
		case EWaveState::Idle:        return TEXT("Idle");
		case EWaveState::Preparing:   return TEXT("Preparing");
		case EWaveState::Spawning:    return TEXT("Spawning");
		case EWaveState::Fighting:    return TEXT("Fighting");
		case EWaveState::WaveCleared: return TEXT("WaveCleared");
		case EWaveState::RunEnded:    return TEXT("RunEnded");
		default:                      return TEXT("Unknown");
		}
	}
}
void ARushBattleGameMode::BeginPlay()
{
    Super::BeginPlay();
	TotalWaves = WaveConfig ? WaveConfig->Waves.Num() : 0;

	const FString ConfigName = WaveConfig ? WaveConfig->GetName() : TEXT("None");
	UE_LOG(LogTemp, Warning, TEXT("[Wave] BeginPlay Config=%s TotalWaves=%d"),
		*ConfigName, TotalWaves);

	StartRun();
}

void ARushBattleGameMode::StartRun()
{
	CurrentWaveIndex = -1;
	EnemiesAlive = 0;

	UE_LOG(LogTemp, Warning, TEXT("[Wave] StartRun TotalWaves=%d"), TotalWaves);

	EnterState(EWaveState::Idle);
}

void ARushBattleGameMode::EnterState(EWaveState NewState)
{
	UE_LOG(LogTemp, Warning, TEXT("[Wave] State %s -> %s WaveIndex=%d"),
		WaveStateToString(WaveState),
		WaveStateToString(NewState),
		CurrentWaveIndex);

	WaveState = NewState;

	OnWaveStateChanged.Broadcast(NewState, CurrentWaveIndex);
}
