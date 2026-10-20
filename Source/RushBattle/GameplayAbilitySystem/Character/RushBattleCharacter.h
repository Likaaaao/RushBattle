// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "RushBattleCharacter.generated.h"

UCLASS()
class RUSHBATTLE_API ARushBattleCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UMyAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
public:
	// Sets default values for this character's properties
	ARushBattleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>>AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RemoveAbility(TArray<FGameplayAbilitySpecHandle>AbilityHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;


};
