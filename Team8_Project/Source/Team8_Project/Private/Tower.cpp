#include "Tower.h"
#include "Components/BoxComponent.h"

ATower::ATower() :
	TowerHP(1000.0f)
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

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


