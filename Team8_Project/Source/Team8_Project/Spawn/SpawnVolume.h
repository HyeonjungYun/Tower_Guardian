#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class ABaseEnemy;
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
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass);

	FVector GetWaypoint(int32 Index) const;
	FVector GetLocationNearWaypoint(int32 Index) const;
	
	int32 WaypointCount() const;
	
private:
	UFUNCTION()
	void AddKillEnemyCount(ABaseEnemy* DeadEnemy);
	
	FVector GetSpawnPosition();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Waypoint", meta=(MakeEditWidget))
	TArray<FVector> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	float WaypointRange;
};
