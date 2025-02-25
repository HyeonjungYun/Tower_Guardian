#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Team8_Project/Enemy/BaseEnemy.h"
#include "Team8_Project/Enemy/EnemyAIController.h"

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


	FVector SpawnPosition = FVector(BoxOrigin.X + FMath::RandRange(-BoxExtent.X, BoxExtent.X),
	                                BoxOrigin.Y + FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
	                                BoxOrigin.Z - BoxExtent.Z);
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

	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(SpawnedActor))
		Enemy->SetPatrolPath(PatrolPath);
	
	return SpawnedActor;
}
