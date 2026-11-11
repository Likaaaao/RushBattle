// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RushBattle/GameplayAbilitySystem/Character/RushBattleCharacter.h"
#include "RushBattlePlayerCharacter.generated.h"

/**
 * PlayerCharacter
 */
class UResourceSet;

UCLASS()
class RUSHBATTLE_API ARushBattlePlayerCharacter : public ARushBattleCharacter
{
	GENERATED_BODY()
public:
    ARushBattlePlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AttributeSet")
	TObjectPtr<UResourceSet> ResourceSet;
public:
	virtual void ApplyInitStats() override;


	UFUNCTION(BlueprintPure, Category = "Player")
	UResourceSet* GetResourceSet() const { return ResourceSet; }


};
