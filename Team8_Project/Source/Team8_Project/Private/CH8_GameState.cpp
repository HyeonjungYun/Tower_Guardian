#include "CH8_GameState.h"
#include "SpawnVolume.h"
#include "Kismet/GameplayStatics.h"

ACH8_GameState::ACH8_GameState() 
	: Score(0)
	, Gold(0)
	, CurrentWaveIndex(0)
	, WaveDuration(5.0f)
	, StartDuration(60.0f)
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
		SpawnStartTimerHandle,
		this,
		&ACH8_GameState::SpawnWave,
		StartDuration,
		false
	);
}

void ACH8_GameState::SpawnWave()
{
	
}

ASpawnVolume* ACH8_GameState::GetSpawnVolume() const
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	return (FoundVolumes.Num() > 0) ? Cast<ASpawnVolume>(FoundVolumes[0]) : nullptr;
}

