// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseItem.h"
#include "OtherItem.generated.h"

UCLASS()
class TEAM8_PROJECT_API AOtherItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AOtherItem();
protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;*/
};
