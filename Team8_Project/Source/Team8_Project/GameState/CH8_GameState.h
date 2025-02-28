#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH8_GameState.generated.h"

class ABaseEnemy;
class ASpawnVolume;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameGetGold, int32);
DECLARE_DELEGATE(FOnGameKillEnemy);

UCLASS()
class TEAM8_PROJECT_API ACH8_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ACH8_GameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float EnemySpawnDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float StartDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ElapsedSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemySpawnConut;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	TArray<TSubclassOf<ABaseEnemy>> EnemyTypes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 Gold;

	UPROPERTY()
	int32 RemainingHeistTime;

	UPROPERTY()
	int32 RemainingInfinityAmmoTime;

	
	int32 SpawnedEnemy;
	int32 KilledEnemy;

	FOnGameGetGold OnGameSetGold;
	FOnGameKillEnemy OnGameKillEnemy;

	FTimerHandle HeistTimerHandle;
	FTimerHandle InfinityAmmoTimerHandle;
	FTimerHandle GameTimerHandle;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle SpawnDurationTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Heist")
	void UseHeistItem();

	UFUNCTION(BlueprintCallable, Category = "InfinityAmmo")
	void UseInfinityAmmoItem();
	
	
	void UpdateHUD();
	void UpdateGameTimer();

	virtual void BeginPlay() override;

	void SetGold(int32 TempGold);
	void StartGame();
	void EndGame();
	void SpawnWave();
	void SpawnEnemyPerTime();
	void UpdatedSpawnedEnemy();
	void UpdatedKilledEnemy();
	void UpdatedHeistDuration();
	void UpdatedInfinityAmmoDuration();
	int32 GetGold();

private:


	// 헬퍼 함수
	ASpawnVolume* GetSpawnVolume() const;
};
