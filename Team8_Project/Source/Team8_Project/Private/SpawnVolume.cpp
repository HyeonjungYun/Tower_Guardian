#include "SpawnVolume.h"
#include "EnemySpawnRow.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	EnemyDataTable = nullptr;
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

TSubclassOf<AActor> ASpawnVolume::SpawnNormalEnemy()
{
	
	if (!EnemyDataTable) return nullptr;

	TArray<FEnemySpawnRow*> AllRows;
	static const FString ContextString(TEXT("EnemySpawnContext"));
	EnemyDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;
	
	if (AllRows.IsValidIndex(0))
	{
		if (TSubclassOf<AActor> NormalEnemy = AllRows[0]->EnemyClass.Get())
		{
			return NormalEnemy;
		}
	}
	return nullptr;
}

