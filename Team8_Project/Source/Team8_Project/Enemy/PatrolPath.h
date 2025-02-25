// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class TEAM8_PROJECT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPath();

	FVector GetWaypoint(int32 Index) const;
	int32 Num() const;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Patrol", meta=(MakeEditWidget))
	TArray<FVector> Waypoints;
	
	
};
