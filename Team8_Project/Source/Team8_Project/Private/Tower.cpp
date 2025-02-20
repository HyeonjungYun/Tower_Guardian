#include "Tower.h"

ATower::ATower() :
	TowerHP(1000.0f)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
	
}

float ATower::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float RealDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetHP(TowerHP - RealDamage);

	return RealDamage;
}

float ATower::GetHP() const
{
	return TowerHP;
}

void ATower::SetHP(float value)
{
	TowerHP = value;
}


