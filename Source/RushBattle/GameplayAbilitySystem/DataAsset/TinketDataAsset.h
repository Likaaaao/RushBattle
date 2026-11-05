// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TinketDataAsset.generated.h"

/**
 * 
 */

class UGameplayEffect;
class UTexture2D;

UENUM(BlueprintType)
enum class ETinketType : uint8
{
	White UMETA(DisplayName = "White"),
    Blue UMETA(DisplayName = "Blue"),
	Purple UMETA(DisplayName = "Purple"),
	Gold UMETA(DisplayName = "Gold")
};

UCLASS()
class RUSHBATTLE_API UTinketDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tinket")
	FText Name;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tinket")
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tinket")
	TObjectPtr<UTexture2D> Icon;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tinket")
    ETinketType TinketType= ETinketType::White;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Trinket")
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tinket")
	float Values=1.f;
};
