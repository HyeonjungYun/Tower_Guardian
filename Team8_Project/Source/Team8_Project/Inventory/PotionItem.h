// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableItem.h"
#include "PotionItem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API APotionItem : public AConsumableItem
{
	GENERATED_BODY()
	
public:
	APotionItem();
protected:
	virtual void ActivateItem(AActor* Activator) override;
	//int32 HealAmount;
};
