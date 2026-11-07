// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattle/GameplayAbilitySystem/Trinket/TrinketPickup.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/TrinketDataAsset.h"

ATrinketPickup::ATrinketPickup()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATrinketPickup::Initialize(UTrinketDataAsset* InTrinket)
{
	TrinketData = InTrinket;
	OnInitialized();
}


