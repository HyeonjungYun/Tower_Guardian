// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsumableItem.h"
#include "BigPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API ABigPotionItem : public AConsumableItem
{
	GENERATED_BODY()
public:
	ABigPotionItem();
protected:
	virtual void ActivateItem(AActor* Activator) override;
	
};
