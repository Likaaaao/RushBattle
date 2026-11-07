// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LootSubSystem.generated.h"

class ATrinketPickup;
class ULootTableDataAsset;
class UTrinketDataAsset;
/**
 * Drop Spawn Factory
 *
 * Turns drop tables / trinket data into world ATrinketPickup instances.
 */
UCLASS()
class RUSHBATTLE_API ULootSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Loot", meta = (WorldContext = "WorldContextObject"))
	static ULootSubSystem* GetLootSubSystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Loot")
	TArray<ATrinketPickup*>SpawnFromTable(ULootTableDataAsset* Table, FVector Origin);



	UFUNCTION(BlueprintCallable, Category = "Loot")
	ATrinketPickup* SpawnFromTrinket(
		UTrinketDataAsset* Trinket, 
		FVector Origin, 
		TSubclassOf<ATrinketPickup> PickupClass);
private:
	// 从 Desired 上方往下打射线，找到地面就贴上去 
	FVector FindGroundLocation(const FVector& Desired) const;
};
