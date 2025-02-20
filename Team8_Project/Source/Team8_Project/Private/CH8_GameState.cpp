#include "CH8_GameState.h"
#include "SpawnVolume.h"
#include "EnemySpawnRow.h"
#include "Team8_Project/Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"

ACH8_GameState::ACH8_GameState() 
	: Score(0)
	, Gold(0)
	, CurrentWaveIndex(0)
	, WaveDuration(5.0f)
	, StartDuration(5.0f)
	, EnemySpawnDuration(0.5f)
	, EnemySpawnConut(0)
{
}

int32 ACH8_GameState::GetScore()
{
	return Score;
}

int32 ACH8_GameState::GetGold()
{
	return Gold;
}

void ACH8_GameState::BeginPlay()
{
	StartGame();
}

void ACH8_GameState::StartGame()
{
	GetWorldTimerManager().SetTimer(
		GameTimerHandle,
		this,
		&ACH8_GameState::SpawnWave,
		WaveDuration,
		true,
		StartDuration
	);
}

void ACH8_GameState::SpawnWave()
{
	GetWorldTimerManager().SetTimer(
		SpawnDurationTimerHandle,
		this,
		&ACH8_GameState::SpawnEnemyPerTime,
		EnemySpawnDuration,
		true,
		0.0f
	);
}

void ACH8_GameState::SpawnEnemyPerTime()
{
	if (EnemySpawnConut < 5)
	{
		if (ASpawnVolume* SpawnVolume = GetSpawnVolume())
		{
			if (TSubclassOf<AActor> Enemy = SpawnVolume->SpawnNormalEnemy())
			{
				UE_LOG(LogTemp, Warning, TEXT("Start Spawn Per Time"));
				SpawnVolume->SpawnEnemy(Enemy);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("EnemySpawnConut : %d"), EnemySpawnConut);
		EnemySpawnConut++;
	}
	else
	{
		EnemySpawnConut = 0;
		GetWorldTimerManager().ClearTimer(SpawnDurationTimerHandle);
	}
}

ASpawnVolume* ACH8_GameState::GetSpawnVolume() const
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	return (FoundVolumes.Num() > 0) ? Cast<ASpawnVolume>(FoundVolumes[0]) : nullptr;
}

