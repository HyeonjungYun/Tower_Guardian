// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldSpawnUISubSystem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API UWorldSpawnUISubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UWorldSpawnUISubSystem();

	void SpawnDamageText(UWorld* World, float Damage, const FVector Location);
	
private:
	TSubclassOf<AActor> DamageTextWidgetActorClass;
};
