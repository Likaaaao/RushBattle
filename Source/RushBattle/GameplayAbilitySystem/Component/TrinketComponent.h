// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "TrinketComponent.generated.h"


class UTinketDataAsset;
class UTrinketPoolDataAsset;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrinketEquipped, UTinketDataAsset*,Trinket,int32,Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTrinketRemoved, UTinketDataAsset*,Trinket,int32,Index);



USTRUCT(BlueprintType)
struct FEquippedTrinket
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trinket")
	TObjectPtr<UTinketDataAsset> Data;

	UPROPERTY()
	FActiveGameplayEffectHandle EffectHandle;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUSHBATTLE_API UTrinketComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrinketComponent();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trinket")
	TObjectPtr<UTrinketPoolDataAsset> TrinketPool;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Trinket")
    TArray<FEquippedTrinket> EquippedTrink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trinket")
	int32 MaxSlots;

public:
	UPROPERTY(BlueprintAssignable, Category = "Trinket")
	FOnTrinketEquipped OnTrinketEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Trinket")
	FOnTrinketRemoved OnTrinketRemoved;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Trinket")
	bool EquipTrinket(UTinketDataAsset* Trinket);

	UFUNCTION(BlueprintCallable, Category = "Trinket")
	UTinketDataAsset* ReplaceTrinket(UTinketDataAsset* Trinket, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Trinket")
	UTinketDataAsset* RemoveTrinket(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Trinket")
	void ClearAllTrinkets();

	UFUNCTION(BlueprintPure, Category = "Trinket")
	int32 GetEquippedTrinketCount() const;

	UFUNCTION(BlueprintPure, Category = "Trinket")
	bool HasEmptySlot() const;
private:
	UAbilitySystemComponent* GetOwnerASC() const;

	void ApplyEffect(FEquippedTrinket& EquippedTrinket);
    void RemoveEffect(FEquippedTrinket& EquippedTrinket);
		
};
