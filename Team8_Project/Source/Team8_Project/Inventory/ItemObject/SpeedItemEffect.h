// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffectBase.h"
#include "SpeedItemEffect.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API USpeedItemEffect : public UItemEffectBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float SpeedAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration = 0.f;

	virtual void ApplyItemEffect(AActor* Target) override;
};
