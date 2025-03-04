// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemEffectBase.generated.h"


UCLASS(Abstract)
class TEAM8_PROJECT_API UItemEffectBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void ApplyItemEffect(AActor* Target) PURE_VIRTUAL(UItemEffectBase::ApplyEffect_Implementation, );
};
