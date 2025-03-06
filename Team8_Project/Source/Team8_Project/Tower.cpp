#include "Tower.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/GameState/CH8_GameState.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

ATower::ATower() 
	: TowerHP(1000.0f)
	, TowerMaxHP(1000.0f)
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->SetupAttachment(Scene);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	TowerHPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerHPBar"));
	TowerHPBar->SetupAttachment(Scene);

	PrimaryActorTick.bCanEverTick = false;
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
	
}

float ATower::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EventInstigator->GetPawn()->IsA<AMyCharacter>())
	{
		return 0.f;
	}

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

	if (TowerHP <= 0)
	{
		TowerHP = 0;
	}

	if (TowerHP <= 0)
	{
		if (ACH8_GameState* GameState = Cast<ACH8_GameState>(GetWorld()->GetGameState()))
		{
			GameState->EndGame(false);
		}
	}
}


