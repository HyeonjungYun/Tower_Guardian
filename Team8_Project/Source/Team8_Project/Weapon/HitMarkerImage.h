// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitMarkerImage.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API UHitMarkerImage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* HitMarkerImage;

};
