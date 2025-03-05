#include "Team8_Project/GameState/CH8_GameState.h"
#include "Team8_Project/Spawn/SpawnVolume.h"
#include "Team8_Project/Spawn/EnemySpawnRow.h"
#include "Team8_Project/MyPlayerController.h"
#include "Team8_Project/Enemy/BaseEnemy.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACH8_GameState::ACH8_GameState()
	: Score(0)
	, Gold(1000)
	, CurrentWaveIndex(10)
	, WaveDuration(5.0f)
	, StartDuration(5.0f)
	, EnemySpawnDuration(0.5f)
	, EnemySpawnConut(0)
	, ElapsedSeconds(0)
	, SpawnedEnemy(0)
	, KilledEnemy(0)
	, RemainingHeistTime(0)
	, RemainingInfinityAmmoTime(0)
{
	OnGameSetGold.AddUObject(this, &ACH8_GameState::SetGold);
	OnGameKillEnemy.BindUObject(this, &ACH8_GameState::UpdatedKilledEnemy);
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

				if (UTextBlock* GoldText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("GoldText"))))
				{
					GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold : %d"), Gold)));
				}

				if (UTextBlock* TempEnemyText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TempEnemyText"))))
				{
					TempEnemyText->SetText(FText::FromString(FString::Printf(TEXT("KilledEnemy / SpawnedEnemy :\n%d\t\t\t/ %d"), KilledEnemy, SpawnedEnemy)));
				}

				if (GetWorldTimerManager().IsTimerActive(HeistTimerHandle))
				{
					if (UTextBlock* HeistTime = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HeistTime"))))
					{
						if (RemainingHeistTime <= 0)
						{
							RemainingHeistTime = 0;
							HeistTime->SetText(FText::FromString(FString::Printf(TEXT(""))));
							GetWorldTimerManager().ClearTimer(HeistTimerHandle);
							return;
						}
						HeistTime->SetText(FText::FromString(FString::Printf(TEXT("Heist : %ds"), RemainingHeistTime)));
					}

				}

				if (GetWorldTimerManager().IsTimerActive(InfinityAmmoTimerHandle))
				{
					if (UTextBlock* InfinityAmmoTime = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("InfinityAmmoTime"))))
					{
						if (RemainingInfinityAmmoTime <= 0)
						{
							RemainingInfinityAmmoTime = 0;
							InfinityAmmoTime->SetText(FText::FromString(FString::Printf(TEXT(""))));
							GetWorldTimerManager().ClearTimer(InfinityAmmoTimerHandle);
							return;
						}
						InfinityAmmoTime->SetText(FText::FromString(FString::Printf(TEXT("Infinity Ammo : %ds"), RemainingInfinityAmmoTime)));
					}
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

void ACH8_GameState::SetGold(int32 TempGold)
{
	Gold += TempGold;
}

void ACH8_GameState::StartGame()
{
	GetWorldTimerManager().SetTimer(
		GameTimerHandle,
		this, 
		&ACH8_GameState::UpdateGameTimer,
		1.0f,
		true
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
		true
	);
}

void ACH8_GameState::SpawnEnemyPerTime()
{
	//플레이어 테스트를 위한 주석
	if (EnemySpawnConut < 5)	// 웨이브 당 생성될 Enemy 숫자는 별도로 수정 필요
	{
		if (ASpawnVolume* SpawnVolume = GetSpawnVolume())
		{
			SpawnVolume->SpawnEnemy(EnemyTypes[0]);
			
			UpdatedSpawnedEnemy();
			EnemySpawnConut++;
		}
	}
	else
	{
		EnemySpawnConut = 0;
		GetWorldTimerManager().ClearTimer(SpawnDurationTimerHandle);
	}
}

void ACH8_GameState::UpdatedSpawnedEnemy()
{
	SpawnedEnemy++;
}

void ACH8_GameState::UpdatedKilledEnemy()
{
	KilledEnemy++;
}

void ACH8_GameState::UpdatedHeistDuration()
{
	if (GetWorldTimerManager().IsTimerActive(HeistTimerHandle))
	{
		RemainingHeistTime--;
	}
}

void ACH8_GameState::UpdatedInfinityAmmoDuration()
{
	if (GetWorldTimerManager().IsTimerActive(InfinityAmmoTimerHandle))
	{
		RemainingInfinityAmmoTime--;
	}
}

int32 ACH8_GameState::GetGold()
{
	return Gold;
}

ASpawnVolume* ACH8_GameState::GetSpawnVolume() const
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	return (FoundVolumes.Num() > 0) ? Cast<ASpawnVolume>(FoundVolumes[0]) : nullptr;
}


void ACH8_GameState::UseHeistItem()
{
	if (GetWorldTimerManager().IsTimerActive(HeistTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(HeistTimerHandle);
	}

	RemainingHeistTime = 90;
	GetWorldTimerManager().SetTimer(
		HeistTimerHandle,
		this,
		&ACH8_GameState::UpdatedHeistDuration,
		1.0f,
		true
	);
}

void ACH8_GameState::UseInfinityAmmoItem()
{
	if (GetWorldTimerManager().IsTimerActive(InfinityAmmoTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(InfinityAmmoTimerHandle);
	}
	
	RemainingInfinityAmmoTime = 5;
	GetWorldTimerManager().SetTimer(
		InfinityAmmoTimerHandle,
		this,
		&ACH8_GameState::UpdatedInfinityAmmoDuration,
		1.0f,
		true
	);
}
