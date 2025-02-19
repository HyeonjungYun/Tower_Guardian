// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BaseEnemy.h"
#include "Engine/TargetPoint.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

}
