// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OtherItem.h"
#include "BananaItem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API ABananaItem : public AOtherItem
{
	GENERATED_BODY()
public:
	ABananaItem();
protected:
	virtual void ActivateItem(AActor* Activator) override;
};
