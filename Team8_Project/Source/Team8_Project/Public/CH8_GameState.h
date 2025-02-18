#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH8_GameState.generated.h"

UCLASS()
class TEAM8_PROJECT_API ACH8_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ACH8_GameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 Gold;

	FTimerHandle GameTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore();

	UFUNCTION(BlueprintPure, Category = "Gold")
	int32 GetGold();

	void StartGame();
	void EndGame();
	void SpawnWave(TArray<AActor*> MinionToSpawnPerWave);

private:
	UPROPERTY()
	TArray<AActor*> MinionToSpawnPerWave;
};
