#include "CH8_GameState.h"
#include "SpawnVolume.h"
#include "EnemySpawnRow.h"
#include "Team8_Project/MyPlayerController.h"
#include "Team8_Project/Enemy/BaseEnemy.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACH8_GameState::ACH8_GameState() 
	: Score(0)
	, Gold(0)
	, CurrentWaveIndex(0)
	, WaveDuration(5.0f)
	, StartDuration(5.0f)
	, EnemySpawnDuration(0.5f)
	, EnemySpawnConut(0)
	, ElapsedSeconds(0)
{
}

void ACH8_GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("GameTimerText"))))
				{
					int TempMin = ElapsedSeconds / 60;
					int TempSecond = ElapsedSeconds % 60;
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), TempMin, TempSecond)));
				}

				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaveText"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d"), CurrentWaveIndex)));
				}
			}
		}
	}
}

void ACH8_GameState::UpdateGameTimer()
{
	ElapsedSeconds++;
	UpdateHUD();
}

void ACH8_GameState::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACH8_GameState::UpdateHUD,
		0.1f,
		true
	);

	StartGame();
}

void ACH8_GameState::StartGame()
{
	GetWorldTimerManager().SetTimer(
		GameTimerHandle,
		this, 
		&ACH8_GameState::UpdateGameTimer,
		1.0f,
		true,
		0.0f
	);

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
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
				SpawnVolume->SpawnEnemy(Enemy);
			}
		}

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

