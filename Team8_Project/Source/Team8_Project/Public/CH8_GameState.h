#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH8_GameState.generated.h"

class ASpawnVolume;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemySpawnConut;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 Gold;

	FTimerHandle GameTimerHandle;
	FTimerHandle SpawnDurationTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	void UpdateEnemyAtHUD();

	UFUNCTION(BlueprintPure, Category = "Gold")
	void UpdateGoldAtHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<AActor*> MinionToSpawnPerWave;

	virtual void BeginPlay() override;

	void StartGame();
	void EndGame();
	void SpawnWave();
	void SpawnEnemyPerTime();

private:

	// 헬퍼 함수
	ASpawnVolume* GetSpawnVolume() const;
};
