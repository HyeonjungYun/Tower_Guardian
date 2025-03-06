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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave")
	int32 GameClearWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gold")
	int32 Gold;

	UPROPERTY()
	int32 RemainingHeistTime;

	UPROPERTY()
	int32 RemainingInfinityAmmoTime;

	UPROPERTY(EditAnywhere, Category = "GameUI")
	TSubclassOf<UUserWidget> StartWidgetClass;

	UPROPERTY()
	UUserWidget* StartWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "GameUI")
	TSubclassOf<UUserWidget> EndWidgetClass;

	UPROPERTY()
	UUserWidget* EndWidgetInstance;

	
	int32 SpawnedEnemy;
	int32 KilledEnemy;
	int32 NumberOfEnemy;
	int32 NumberOfEnemyMax;

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

	UFUNCTION(BlueprintImplementableEvent, Category = "NeutralEnemy")
	void SpawnNeutralEnemy();

	UFUNCTION(BlueprintCallable, Category = "GameStart")
	void StartGame();


	void OpenStartWidget();
	void OpenEndWidget();
	
	void UpdateHUD();
	void UpdateGameTimer();

	virtual void BeginPlay() override;
	void GetScore();
	void SetGold(int32 TempGold);
	void EndGame(bool bIsGameClear);
	void SpawnWave();
	void SpawnEnemyPerTime();
	void UpdatedSpawnedEnemy();
	void UpdatedKilledEnemy();
	void UpdatedHeistDuration();
	void UpdatedInfinityAmmoDuration();
	int32 GetGold();

	UPROPERTY(BlueprintAssignable, Category = "Gold")
	FOnGoldChanged OnGoldChanged;
private:


	// 헬퍼 함수
	TArray<ASpawnVolume*> GetSpawnVolume() const;

	UFUNCTION(BlueprintCallable, Category = "NeutralEnemy")
	ANeutralMonsterSpawnVolume* GetNeutralEnemySpawnVolume() const;
};
