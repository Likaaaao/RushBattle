// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "RushBattleCharacter.generated.h"
class ARushBattleCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnKilledEnemy,
	ARushBattleCharacter*, Victim);

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()

	//	Basic Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Speed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxSpeed = 600.f;

	//  Attributes|Resource 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Ammo = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxAmmo = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Energy = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxEnergy = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Money = 0.f;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	FCharacterAttributes InitialAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> LastDamageInstigator;

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnKilledEnemy OnKilledEnemy;
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

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyInitialAttributes();

	UFUNCTION(BlueprintCallable, Category = "GAS")
	FActiveGameplayEffectHandle ApplyEffectToSelfByCaller(
		TSubclassOf<UGameplayEffect> EffectClass,
		FGameplayTag DataTag,
		float Magnitude,
		float Level = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "GAS|Combat")
	FActiveGameplayEffectHandle DealDamageTo(
		ARushBattleCharacter* Target,
		TSubclassOf<UGameplayEffect> EffectClass,
		FGameplayTag DataTag,
		float Magnitude =1.0f,
		AActor* Causer = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void NotifyKilledEnemy(ARushBattleCharacter* Victim);

};
