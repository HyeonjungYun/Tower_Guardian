// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableItemData.h"
#include "PotionItemData.generated.h"

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	BigPotion,
	SmallPotion
};

UCLASS()
class TEAM8_PROJECT_API UPotionItemData : public UConsumableItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Item")
	EPotionType PotionType;
};
