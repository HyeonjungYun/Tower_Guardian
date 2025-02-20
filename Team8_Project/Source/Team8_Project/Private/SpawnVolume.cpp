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
	UE_LOG(LogTemp, Warning, TEXT("Check EnemyClass"));
	if (!EnemyClass)
	{
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn Enemy Start"));
	FVector SpawnPosition = GetSpawnPosition();

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		SpawnPosition,
		FRotator::ZeroRotator
	);
	UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
	return SpawnedActor;
}

TSubclassOf<AActor> ASpawnVolume::SpawnNormalEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Check"));
	if (!EnemyDataTable) return nullptr;

	TArray<FEnemySpawnRow*> AllRows;
	static const FString ContextString(TEXT("EnemySpawnContext"));
	EnemyDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Spawn Start"));
	if (AllRows.IsValidIndex(0))
	{
		if (TSubclassOf<AActor> NormalEnemy = AllRows[0]->EnemyClass.Get())
		{
			return NormalEnemy;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn Check Failed"));
	return nullptr;
}

