// Fill out your copyright notice in the Description page of Project Settings.


#include "RushBattle/GameplayAbilitySystem/Weapon/WeaponBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::EquipWeapon_Implementation()
{

}

void AWeaponBase::UnequipWeapon_Implementation()
{

}

void AWeaponBase::InitWeapon_Implementation()
{

}

UAbilitySystemComponent* AWeaponBase::GetOnerASC() const
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) {
		return nullptr;
	}
	//priority interface 
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerActor)) {
		return ASI->GetAbilitySystemComponent();
	}
	//back to find ASC
	return OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
}

void AWeaponBase::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!AbilityClass) {
		return;
	}
	UAbilitySystemComponent* ASC = GetOnerASC();
	if (!ASC) {
		UE_LOG(LogTemp, Warning,
			TEXT("[WeaponBase] %s 取不到 Owner 的 ASC，能力 %s 未授予（Owner=%s）"),
			*GetName(), *GetNameSafe(AbilityClass), *GetNameSafe(GetOwner()));
		return;
	}
	//去重
	for (const FGameplayAbilitySpec& spec : ASC->GetActivatableAbilities()) {
		if (spec.Ability && spec.Ability->GetClass() == AbilityClass.Get()) {
			return;
		}
	}
    FGameplayAbilitySpec spec= FGameplayAbilitySpec(AbilityClass, 1, -1, this);
	const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(spec);
	if (Handle.IsValid()) {
		GrantedAbilityHandles.Add(Handle);
	}
}

void AWeaponBase::GrantAbilitiesToOwner(TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities)
{
	for (TSubclassOf<UGameplayAbility> ability : GrantedAbilities) {
		GrantAbility(ability);
	}
	
}

void AWeaponBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> RemoveGrantedAbilityHandles)
{
	UAbilitySystemComponent* ASC = GetOnerASC();
	if (!ASC) {
		return;
	}
	for (const FGameplayAbilitySpecHandle Handle : RemoveGrantedAbilityHandles) {
		if (Handle.IsValid()) {
			ASC->ClearAbility(Handle);
		}
	}

}



