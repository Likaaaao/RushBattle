// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattle/GameplayAbilitySystem/Trinket/LootSubSystem.h"
#include "Engine/World.h"
#include "RushBattle/GameplayAbilitySystem/Trinket/TrinketPickup.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/LootTableDataAsset.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/TrinketPoolDataAsset.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/TrinketDataAsset.h"

ULootSubSystem* ULootSubSystem::GetLootSubSystem(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	return World ? World->GetSubsystem<ULootSubSystem>() : nullptr;;
}

TArray<ATrinketPickup*> ULootSubSystem::SpawnFromTable(ULootTableDataAsset* Table, FVector Origin)
{
	TArray<ATrinketPickup*>Result;
	if (!Table || !Table->TrinketPool) {
		return Result;
	}

	UWorld* World = GetWorld();
	if (!Table->PickupClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Loot] Table has no PickupClass, skip."));
		return Result;
	}
	if (!World) {
		return Result;
	}
	const int32 Count= FMath::RandRange(Table->MinCount,Table->MaxCount);
	if (Count <= 0) {
		return Result;
	}

	TArray<UTrinketDataAsset*> Trinkets = Table->TrinketPool->RollTrinketCount(Count);
	if (Trinkets.Num() == 0) {
		return Result;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (INT32 i = 0; i < Trinkets.Num(); i++) {
		//get location
		const float Angle = (2.f * PI * i) / Trinkets.Num();
		const FVector Offset(
			FMath::Cos(Angle) * Table->Radius,
			FMath::Sin(Angle) * Table->Radius,
			0.f);
		const FVector SpawnLoc = FindGroundLocation(Origin + Offset);
		//spawn
		ATrinketPickup* Pickup = World->SpawnActor<ATrinketPickup>(
			Table->PickupClass, SpawnLoc, FRotator::ZeroRotator, Params);

		if (!Pickup) {
			UE_LOG(LogTemp, Warning,
				TEXT("[Loot] SpawnActor failed at %s"), *SpawnLoc.ToString());
			continue;
		}
		Pickup->Initialize(Trinkets[i]);
        Result.Add(Pickup);
	}
	return Result;
}

ATrinketPickup* ULootSubSystem::SpawnFromTrinket(UTrinketDataAsset* Trinket, FVector Origin, TSubclassOf<ATrinketPickup> PickupClass)
{
	if (!Trinket) {
        return nullptr;
	}
	if (!PickupClass) {
		return nullptr;
	}
	UWorld*World = GetWorld();
	if (!World) {
		return nullptr;
	}
	const FVector SpawnLoc = FindGroundLocation(Origin);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATrinketPickup* Pickup = World->SpawnActor<ATrinketPickup>(
		PickupClass, SpawnLoc, FRotator::ZeroRotator, Params);
	if (!Pickup)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Loot] SpawnActor failed at %s"), *SpawnLoc.ToString());
		return nullptr;
	}
	Pickup->Initialize(Trinket);
	return Pickup;
}

FVector ULootSubSystem::FindGroundLocation(const FVector& Desired) const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return Desired;
	}

	const FVector Start = Desired + FVector(0.f, 0.f, 200.f);
	const FVector End = Desired - FVector(0.f, 0.f, 500.f);

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(LootGroundTrace), false);

	// 只查静态几何：地面和墙
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		return Hit.ImpactPoint + FVector(0.f, 0.f, 50.f);
	}

	return Desired;
}
