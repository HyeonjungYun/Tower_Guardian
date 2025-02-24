// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ConsumableItem.generated.h"




UCLASS()
class TEAM8_PROJECT_API AConsumableItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AConsumableItem();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	//
};
