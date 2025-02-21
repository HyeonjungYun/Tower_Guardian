// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "ConsumableItemData.generated.h"

UCLASS()
class TEAM8_PROJECT_API UConsumableItemData : public UItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category = "Item")
	float Amount;
	
};
