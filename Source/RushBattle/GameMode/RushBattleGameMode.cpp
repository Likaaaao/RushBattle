// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattleGameMode.h"
#include "RushBattle/GameplayAbilitySystem/Character/RushBattleEnemyCharacter.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/InitStatsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "TimerManager.h"

namespace
{
	constexpr float SpawnZOffset = 90.f;
}

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
void ARushBattleGameMode::CheckWaveCleared()
{
	int32 Alive = 0;

	//get player location
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	const bool bHasPlayer = IsValid(PlayerPawn);
	const FVector PlayerLocation = bHasPlayer
		? PlayerPawn->GetActorLocation()
		: FVector::ZeroVector;

	for (const TWeakObjectPtr<ARushBattleEnemyCharacter>& enemy : SpawnedEnemies) {
		if (enemy.IsValid()) {

			const float DistSq = FVector::Dist(
				enemy->GetActorLocation(),
				PlayerLocation);

			//MaxDistanceFromPlayer
			if (DistSq > 10000.f) {
				enemy->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("[Wave] Distince too far, Destroy Enemy: %.2f"),DistSq);
				continue;
			}
			Alive++;
		}

	}
	EnemiesAlive = Alive;
	if (Alive > 0) {
		return;
	}
	GetWorldTimerManager().ClearTimer(CheckClearTimerHandle);
	OnWaveCleared();

}
void ARushBattleGameMode::StartNextWave()
{
	StartWave(CurrentWaveIndex+1);
}

void ARushBattleGameMode::OnWaveCleared()
{
	const float Interval = WaveConfig ? WaveConfig->WaveInterval : 3.f;

	UE_LOG(LogTemp, Warning, TEXT("[Wave] Wave %d cleared, next in %.2f"),
		CurrentWaveIndex, Interval);

	EnterState(EWaveState::WaveCleared);

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ARushBattleGameMode::StartNextWave,
		Interval,
		false);
}


void ARushBattleGameMode::StartRun()
{
	CurrentWaveIndex = -1;
	EnemiesAlive = 0;

	UE_LOG(LogTemp, Warning, TEXT("[Wave] StartRun TotalWaves=%d"), TotalWaves);

	EnterState(EWaveState::Idle);

	StartWave(0);
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

void ARushBattleGameMode::StartWave(int32 WaveIndex)
{
	if (!WaveConfig || !WaveConfig->Waves.IsValidIndex(WaveIndex))
	{
		EnterState(EWaveState::RunEnded);
		OnRunEnded.Broadcast(true);
		return;
	}
	//updata CurrentWaveIndex 
	CurrentWaveIndex = WaveIndex;

	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle); 
	GetWorldTimerManager().ClearTimer(CheckClearTimerHandle);  
	SpawnedEnemies.Reset();

	const FWaveDefinition& Definition = WaveConfig->Waves[WaveIndex];
	PendingSpawns.Reset();
	for (int32 EntryIndex = 0; EntryIndex < Definition.Spawns.Num(); ++EntryIndex) 
	{
		const FWaveSpawnEntry& Entry = Definition.Spawns[EntryIndex];
		if (!Entry.EnemyClass) 
		{
			continue;
		}
		const int32 Count=FMath::Max(1, Entry.SpawnCount);
		//加入生成队列
		for (int32 i = 0; i < Count; ++i) 
		{
			PendingSpawns.Add(EntryIndex);
		}
	}
	SpawnCursor = 0;
	UE_LOG(LogTemp, Warning, TEXT("[Wave] StartWave %d: %d enemies, delay=%.2f"),
		WaveIndex, PendingSpawns.Num(), Definition.SpawnDelay);

	if (PendingSpawns.Num() == 0) {
		EnterState(EWaveState::Fighting);
		OnWaveAllSpawned();
		return;
	}

	EnterState(EWaveState::Preparing);	
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle, this, &ARushBattleGameMode::BeginSpawning, Definition.SpawnDelay, false);

}

void ARushBattleGameMode::BeginSpawning()
{
	EnterState(EWaveState::Spawning);
	SpawnOneEnemy();
}

FVector ARushBattleGameMode::ResolveSpawnLocation(const FWaveSpawnEntry& Entry) const
{
	if (IsValid(Entry.SpawnPoint))
	{
		return Entry.SpawnPoint->GetActorLocation() + FVector(0.f, 0.f, SpawnZOffset);
	}

	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!IsValid(PlayerPawn))
	{
		return FVector(0.f, 0.f, SpawnZOffset);
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const float Angle = FMath::FRandRange(0.f, 2.f * PI);
	const float Distance = FMath::FRandRange(
		SpawnRadiusMin, FMath::Max(SpawnRadiusMin, SpawnRadiusMax));

	FVector Desired = PlayerLocation
		+ FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * Distance;

	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLocation;
		const FVector QueryExtent(500.f, 500.f, 500.f);
		if (NavSys->ProjectPointToNavigation(Desired, NavLocation, QueryExtent))
		{
			Desired = NavLocation.Location;
		}
	}

	Desired.Z += SpawnZOffset;
	return Desired;
}

void ARushBattleGameMode::SpawnOneEnemy()
{
	//check IsValid
	if (WaveState != EWaveState::Spawning) 
	{
		return;
	}
	if (!WaveConfig || !WaveConfig->Waves.IsValidIndex(CurrentWaveIndex))
	{
		return;
	}
	if (!PendingSpawns.IsValidIndex(SpawnCursor)) 
	{
		OnWaveAllSpawned();
		return;
	}
	const FWaveDefinition& Definition=WaveConfig->Waves[CurrentWaveIndex];
	const int32 EntryIndex=PendingSpawns[SpawnCursor];
	SpawnCursor++;

	if (!Definition.Spawns.IsValidIndex(EntryIndex)) 
	{
		SpawnOneEnemy();
		return;
	}

	const FWaveSpawnEntry& Entry = Definition.Spawns[EntryIndex];
	//get spawn location and Check
	const FVector SpawnLocation = ResolveSpawnLocation(Entry);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner = this;
	//Spawn Enemy
	ARushBattleEnemyCharacter* Enemy = GetWorld()->SpawnActor<ARushBattleEnemyCharacter>(
		Entry.EnemyClass, SpawnLocation, FRotator::ZeroRotator, Params);

	//init enemy
	if (!IsValid(Enemy)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("[Wave] Spawn failed at %s, skipped"),
			*SpawnLocation.ToString());
	}
	else
	{
        EnemiesAlive++;
		SpawnedEnemies.Add(Enemy);
		//check InitStats
		if (Entry.InitStats) {
			Enemy->ApplyInitStatsWith(Entry.InitStats);
		}
	}

	//check next spawn
	if (PendingSpawns.IsValidIndex(SpawnCursor)) 
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&ARushBattleGameMode::SpawnOneEnemy,
			Definition.SpawnInterval,
			false);
	}
	else 
	{
		OnWaveAllSpawned();
	}

}

void ARushBattleGameMode::OnWaveAllSpawned()
{
	UE_LOG(LogTemp, Warning, TEXT("[Wave] Wave %d all spawned, EnemiesAlive=%d"),
		CurrentWaveIndex, EnemiesAlive);

	EnterState(EWaveState::Fighting);

	GetWorldTimerManager().SetTimer(
		CheckClearTimerHandle,
		this,
		&ARushBattleGameMode::CheckWaveCleared,
		0.3f,
		true);
}
