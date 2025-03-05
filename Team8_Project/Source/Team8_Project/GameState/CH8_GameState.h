#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH8_GameState.generated.h"

class ABaseEnemy;
class ASpawnVolume;
class ANeutralMonsterSpawnVolume;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGoldChanged);

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
	float SpawnNeutralEnemyTime;

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

	FTimerHandle HeistTimerHandle;
	FTimerHandle InfinityAmmoTimerHandle;
	FTimerHandle GameTimerHandle;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle SpawnDurationTimerHandle;
	FTimerHandle SpawnNeutralEnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Heist")
	void UseHeistItem();

	UFUNCTION(BlueprintCallable, Category = "InfinityAmmo")
	void UseInfinityAmmoItem();
	
	
	void UpdateHUD();
	void UpdateGameTimer();

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "InfinityAmmo")
	void SetGold(int32 TempGold);
	void StartGame();
	void EndGame();
	void SpawnWave();
	void SpawnEnemyPerTime();
	void UpdatedSpawnedEnemy();
	void SpawnNeutralEnemy();
	void UpdatedKilledEnemy();
	void UpdatedHeistDuration();
	void UpdatedInfinityAmmoDuration();
	int32 GetGold();

	UPROPERTY(BlueprintAssignable, Category = "Gold")
	FOnGoldChanged OnGoldChanged;
private:


	// 헬퍼 함수
	TArray<ASpawnVolume*> GetSpawnVolume() const;
	ANeutralMonsterSpawnVolume* GetNeutralEnemySpawnVolume() const;
};
