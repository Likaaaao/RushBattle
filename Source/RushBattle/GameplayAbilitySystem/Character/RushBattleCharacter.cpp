// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattleCharacter.h"
#include "RushBattle/GameplayAbilitySystem/MyAttributeSet.h"

// Sets default values
ARushBattleCharacter::ARushBattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet= CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void ARushBattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	GrantAbilities(StartingAbilities);
}

// Called every frame
void ARushBattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARushBattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TArray<FGameplayAbilitySpecHandle> ARushBattleCharacter::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>>AbilitiesToGrant) {
	if (!AbilitySystemComponent) {
		return TArray<FGameplayAbilitySpecHandle>();
	}
	TArray<FGameplayAbilitySpecHandle> Abilities;
	for (TSubclassOf<UGameplayAbility> Ability : AbilitiesToGrant) {
		if (!Ability) {
			continue;
		}
        FGameplayAbilitySpecHandle AbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));
		Abilities.Add(AbilityHandle);

	}
	return Abilities;
}

void ARushBattleCharacter::RemoveAbility(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!AbilitySystemComponent) {
		return;
	}
	for (FGameplayAbilitySpecHandle Handle : AbilityHandlesToRemove) {
        AbilitySystemComponent->ClearAbility(Handle);
	}
	//Ability Changged
}


UAbilitySystemComponent* ARushBattleCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
