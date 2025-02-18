#include "BaseEnemy.h"

#include "EnemyAIController.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	
}