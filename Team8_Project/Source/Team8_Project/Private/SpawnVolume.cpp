#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

FVector ASpawnVolume::GetSpawnPosition()
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	FVector SpawnPosition = FVector(BoxOrigin.X, BoxOrigin.Y - BoxExtent.Y, BoxOrigin.Z);
	
	return SpawnPosition;
}

AActor* ASpawnVolume::SpawnEnemy(TSubclassOf<AActor> EnemyClass)
{
	if (!EnemyClass)
	{
		return nullptr;
	}

	FVector SpawnPosition = GetSpawnPosition();

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		SpawnPosition,
		FRotator::ZeroRotator
	);

	return SpawnedActor;
}

