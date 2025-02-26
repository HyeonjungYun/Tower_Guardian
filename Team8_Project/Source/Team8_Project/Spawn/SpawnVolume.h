#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBehaviorTree;
class APatrolPath;
class UBoxComponent;

UCLASS()
class TEAM8_PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ASpawnVolume();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning")
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBehaviorTree* BehaviorTree;
	
	FVector GetSpawnPosition();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass);
};
