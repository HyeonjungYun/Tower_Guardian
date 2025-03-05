// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseItem.h"
#include "SniperAmmoItem.generated.h"

UCLASS()
class TEAM8_PROJECT_API ASniperAmmoItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ASniperAmmoItem();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	//UStaticMeshComponent* StaticMesh;
	
};
