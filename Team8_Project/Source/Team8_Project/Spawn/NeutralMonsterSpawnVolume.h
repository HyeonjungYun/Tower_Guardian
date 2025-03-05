#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeutralMonsterSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class TEAM8_PROJECT_API ANeutralMonsterSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ANeutralMonsterSpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass);

protected:
	FVector GetSpawnPosition();
};
