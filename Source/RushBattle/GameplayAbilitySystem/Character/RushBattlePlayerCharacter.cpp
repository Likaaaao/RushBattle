#include "RushBattlePlayerCharacter.h"

#include "AbilitySystemComponent.h"

#include "RushBattle/GameplayAbilitySystem/DataAsset/InitStatsDataAsset.h"
#include "RushBattle/GameplayAbilitySystem/ResourceSet.h"

ARushBattlePlayerCharacter::ARushBattlePlayerCharacter()
{
	ResourceSet = CreateDefaultSubobject<UResourceSet>(TEXT("ResourceSet"));
}

void ARushBattlePlayerCharacter::ApplyInitStats()
{
	Super::ApplyInitStats();

	if (!ResourceSet)
	{
		return;
	}

	const UPlayerInitStats* P = Cast<UPlayerInitStats>(InitStats);
	if (!P)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[InitStats] %s: InitStats is not UPlayerInitStats"),
			*GetNameSafe(this));
		return;
	}

	ResourceSet->SetMaxAmmo(P->MaxAmmo);
	ResourceSet->SetAmmo(FMath::Clamp(P->Ammo, 0.f, P->MaxAmmo));

	ResourceSet->SetMaxEnergy(P->MaxEnergy);
	ResourceSet->SetEnergy(FMath::Clamp(P->Energy, 0.f, P->MaxEnergy));

	ResourceSet->SetMoney(P->Money);

	ResourceSet->SetMaxHealthPoint(P->MaxHealthPoint);
	ResourceSet->SetHealthPoint(FMath::Clamp(P->HealthPoint, 0.f, P->MaxHealthPoint));

	ResourceSet->SetMaxAmmoPoint(P->MaxAmmoPoint);
	ResourceSet->SetAmmoPoint(FMath::Clamp(P->AmmoPoint, 0.f, P->MaxAmmoPoint));

	ResourceSet->SetMaxEnergyPoint(P->MaxEnergyPoint);
	ResourceSet->SetEnergyPoint(FMath::Clamp(P->EnergyPoint, 0.f, P->MaxEnergyPoint));
}
