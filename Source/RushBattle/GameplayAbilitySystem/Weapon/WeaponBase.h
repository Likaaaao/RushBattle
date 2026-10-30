// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GameplayAbilitySpec.h"
#include "Engine/DataAsset.h"
#include "WeaponBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;

UCLASS()
class RUSHBATTLE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="Ability")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

    UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly,Category="Ability")
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	UPrimaryDataAsset * WeaponData;
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable,Category="Ability")
	void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GrantAbilitiesToOwner(TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle>RemoveGrantedAbilityHandles);

    UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Weapon")
	void EquipWeapon();
	virtual void EquipWeapon_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void UnequipWeapon();
	virtual void UnequipWeapon_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void InitWeapon();
	virtual void InitWeapon_Implementation();


private:
	UAbilitySystemComponent* GetOnerASC()const;

	 
};
