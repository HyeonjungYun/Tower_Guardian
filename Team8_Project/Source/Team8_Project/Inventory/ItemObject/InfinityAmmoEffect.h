// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffectBase.h"
#include "InfinityAmmoEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TEAM8_PROJECT_API UInfinityAmmoEffect : public UItemEffectBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration = 10.f;

	virtual void ApplyItemEffect(AActor* Target) override;
	
};
