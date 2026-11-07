// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattle/GameplayAbilitySystem/DataAsset/TrinketPoolDataAsset.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/TrinketDataAsset.h"

UTrinketDataAsset* UTrinketPoolDataAsset::RollTrinketOne()const
{
    if (TrinketPool.Num() == 0) {
        return nullptr;
    }
    return TrinketPool[FMath::RandRange(0, TrinketPool.Num() - 1)];
}

TArray<UTrinketDataAsset*> UTrinketPoolDataAsset::RollTrinketCount(int32 count)const
{
    if (TrinketPool.Num() == 0) {
        return TArray<UTrinketDataAsset*>();
    }
    TArray<UTrinketDataAsset*>candidates;
    for (UTrinketDataAsset* t : TrinketPool) {
        if (t) {
            candidates.Add(t);
        }
    }
    TArray<UTrinketDataAsset*>result;
    for (int32 i = 0; i < count && candidates.Num() > 0; i++) {
        int32 x=FMath::RandRange(0, candidates.Num() - 1);
        result.Add(candidates[x]);
        candidates.RemoveAt(x);
    }
    return result;
}
