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

void ARushBattleCharacter::ApplyInitialAttributes()
{
	if (!AbilitySystemComponent || !AttributeSet) {
		return;
	}
	AttributeSet->SetMaxHealth(InitialAttributes.MaxHealth);
    AttributeSet->SetHealth(InitialAttributes.Health);
    AttributeSet->SetMaxSpeed(InitialAttributes.MaxSpeed);
    AttributeSet->SetSpeed(InitialAttributes.Speed);

}



FActiveGameplayEffectHandle ARushBattleCharacter::ApplyEffectToSelfByCaller(TSubclassOf<UGameplayEffect> EffectClass, FGameplayTag DataTag, float Magnitude, float Level)
{
	if (!AbilitySystemComponent || !EffectClass) {
		return FActiveGameplayEffectHandle();
	}
	//context
	FGameplayEffectContextHandle Context= AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	//Spec
	FGameplayEffectSpecHandle SpecHandle=AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, Context);
	if (!SpecHandle.IsValid()) {
		return FActiveGameplayEffectHandle();
	}
	//setByCaller
	SpecHandle.Data->SetSetByCallerMagnitude(DataTag, Magnitude);
	//apply
	return AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FActiveGameplayEffectHandle ARushBattleCharacter::DealDamageTo(
	ARushBattleCharacter* Target,
	TSubclassOf<UGameplayEffect> EffectClass,
	FGameplayTag DataTag,
	float Magnitude,
	AActor* Causer)
{
	if (!Target || !Target->AbilitySystemComponent || !EffectClass)
	{
		return FActiveGameplayEffectHandle();
	}


	// Context
	FGameplayEffectContextHandle Context =
		Target->AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);
	Context.AddInstigator(this, Causer ? Causer : this);

	// Spec
	FGameplayEffectSpecHandle SpecHandle =
		Target->AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, Context);

	if (!SpecHandle.IsValid())
	{
		return FActiveGameplayEffectHandle();
	}
	// LastDamageInstigator
	Target->LastDamageInstigator = this;

	SpecHandle.Data->SetSetByCallerMagnitude(DataTag, Magnitude);

	// apply
	return Target->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ARushBattleCharacter::NotifyKilledEnemy(ARushBattleCharacter* Victim)
{
	if (!Victim)
	{
		return;
	}

	OnKilledEnemy.Broadcast(Victim);
}