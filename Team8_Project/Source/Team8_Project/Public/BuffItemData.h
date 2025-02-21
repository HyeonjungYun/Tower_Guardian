// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableItemData.h"
#include "BuffItemData.generated.h"

UENUM(BlueprintType)
enum class EEBuffType : uint8
{
	Buff,
	Debuff
};

UCLASS()
class TEAM8_PROJECT_API UBuffItemData : public UConsumableItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item")
	EEBuffType BuffType;

	UPROPERTY(EditAnywhere, Category = "Item")
	float Duration;
	
};
