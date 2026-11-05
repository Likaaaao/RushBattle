// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattle/GameplayAbilitySystem/Component/TrinketComponent.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/TrinketPoolDataAsset.h"
#include "RushBattle/GameplayAbilitySystem/DataAsset/TinketDataAsset.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
// Sets default values for this component's properties
UTrinketComponent::UTrinketComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTrinketComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTrinketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTrinketComponent::EquipTrinket(UTinketDataAsset* Trinket)
{
	if (!Trinket) {
		return false;
	}

	if (!HasEmptySlot()) {
		return false;
	}
	FEquippedTrinket NewTrinket;
    NewTrinket.Data = Trinket;
	ApplyEffect(NewTrinket);

	const int32 SlotIndex= EquippedTrink.Add(NewTrinket);
	OnTrinketEquipped.Broadcast(Trinket,SlotIndex);
	return true;
}

UTinketDataAsset* UTrinketComponent::ReplaceTrinket(UTinketDataAsset* Trinket, int32 Index)
{
	//check
	if (!Trinket || !EquippedTrink.IsValidIndex(Index)) {
		return nullptr;
	}
	//saved
	UTinketDataAsset* OldTrinket = EquippedTrink[Index].Data;
    RemoveEffect(EquippedTrink[Index]);
	//equip
	FEquippedTrinket NewTrinket;
    NewTrinket.Data = Trinket;
	ApplyEffect(NewTrinket);
	EquippedTrink[Index] = NewTrinket;
	//Broadcast
	OnTrinketEquipped.Broadcast(Trinket, Index);
	return nullptr;
}

UTinketDataAsset* UTrinketComponent::RemoveTrinket(int32 Index)
{
	if (!EquippedTrink.IsValidIndex(Index)) {
		return nullptr;
	}
	UTinketDataAsset* OldTrinket = EquippedTrink[Index].Data;
	RemoveEffect(EquippedTrink[Index]);
	EquippedTrink.RemoveAt(Index);

	if (OldTrinket) {
		OnTrinketRemoved.Broadcast(OldTrinket, Index);
	}
	return OldTrinket;
}

void UTrinketComponent::ClearAllTrinkets()
{
	for (int32 i = 0; i < EquippedTrink.Num(); i++) {
		if (EquippedTrink[i].Data) {
			OnTrinketRemoved.Broadcast(EquippedTrink[i].Data, i);
		}
		RemoveEffect(EquippedTrink[i]);
	}
	EquippedTrink.Empty();
}

int32 UTrinketComponent::GetEquippedTrinketCount() const
{
	return EquippedTrink.Num();
}

bool UTrinketComponent::HasEmptySlot() const
{
	return EquippedTrink.Num() < MaxSlots;
}

//--------------GameplayAbilitySystem-----------------
UAbilitySystemComponent* UTrinketComponent::GetOwnerASC() const
{
	AActor* OwnerActor= GetOwner();
	if (!OwnerActor) {
		return nullptr;
	}
	if (IAbilitySystemInterface*ASI=Cast<IAbilitySystemInterface>(OwnerActor)) {
		return ASI->GetAbilitySystemComponent();
	}
	return OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
}

void UTrinketComponent::ApplyEffect(FEquippedTrinket& Trinket)
{
	if (!Trinket.Data || !Trinket.Data->EffectToApply) {
        return;
	}

	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC) {
		return;
	}

	FGameplayEffectContextHandle Ctx=ASC->MakeEffectContext();
	Ctx.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec=ASC->MakeOutgoingSpec(Trinket.Data->EffectToApply, 1, Ctx);
	if (!Spec.IsValid()) {
		return;
	}

	Trinket.EffectHandle=ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data);
}

void UTrinketComponent::RemoveEffect(FEquippedTrinket& Trinket)
{
	if (!Trinket.EffectHandle.IsValid()) {
		return;
	}
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (ASC) {
		ASC->RemoveActiveGameplayEffect(Trinket.EffectHandle);
	}
	Trinket.EffectHandle.Invalidate();
}


