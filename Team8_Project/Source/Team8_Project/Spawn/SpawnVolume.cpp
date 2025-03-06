#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Team8_Project/Enemy/BaseEnemy.h"
#include "Team8_Project/Enemy/EnemyAIController.h"
#include "Team8_Project/GameState/CH8_GameState.h"

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
	{
		Enemy->SetSpawnVolume(this);
		Enemy->OnDeath.AddUObject(this, &ASpawnVolume::AddKillEnemyCount);
	}
	
	return SpawnedActor;
}

FVector ASpawnVolume::GetWaypoint(int32 Index) const
{
	if (Waypoints.IsValidIndex(Index))
		return GetActorLocation() + GetActorRotation().RotateVector(Waypoints[Index] * GetActorScale());

	UE_LOG(LogTemp, Warning, TEXT("Waypoint 반환 실패"));
	return FVector::Zero();
}

FVector ASpawnVolume::GetLocationNearWaypoint(int32 Index) const
{
	FVector Result = GetWaypoint(Index);
	Result += FVector(FMath::RandPointInCircle(WaypointRange), 0);

	// DrawDebugSphere(GetWorld(), Result, 40, 10, FColor::Red, false, 10);
	
	return Result;
}

int32 ASpawnVolume::WaypointCount() const
{
	return Waypoints.Num();
}

void ASpawnVolume::AddKillEnemyCount(ABaseEnemy* DeadEnemy)
{
	if (ACH8_GameState* GameState = GetWorld()->GetGameState<ACH8_GameState>())
		GameState->UpdatedKilledEnemy();
}