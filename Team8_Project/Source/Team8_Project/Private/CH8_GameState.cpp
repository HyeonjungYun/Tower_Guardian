#include "CH8_GameState.h"
#include "SpawnVolume.h"
#include "Kismet/GameplayStatics.h"

ACH8_GameState::ACH8_GameState()
	: Score(0)
	, Gold(0)
	, CurrentWaveIndex(0)
	, WaveDuration(5.0f)
	, StartDuration(60.0f)
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
		&ACH8_GameState::SpawnEnemy,
		EnemySpawnDuration,
		true,
		StartDuration
	);
}

void ACH8_GameState::SpawnEnemy()
{
	if (EnemySpawnConut < MinionToSpawnPerWave.GetAllocatedSize())
	{
		if (ASpawnVolume* SpawnVolume = GetSpawnVolume())
		{
			if (MinionToSpawnPerWave.IsValidIndex(EnemySpawnConut))
			{
				//SpawnVolume->SpawnEnemy(MinionToSpawnPerWave[EnemySpawnConut]);
			}
		}

		EnemySpawnConut++;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnDurationTimerHandle);
	}
}

ASpawnVolume* ACH8_GameState::GetSpawnVolume() const
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	return (FoundVolumes.Num() > 0) ? Cast<ASpawnVolume>(FoundVolumes[0]) : nullptr;
}

