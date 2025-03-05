// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitScanWeaponBase.h"
#include "ShotgunBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API AShotgunBase : public AHitScanWeaponBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	int32 GetNumOfPellets();

	UFUNCTION(BlueprintCallable)
	void SetNumOfPellets(int32 NumOfPellets);

	virtual void Fire(const FVector& HitTarget, float CurrentWeaponSpread); // 히트스캔 무기 발사 재정의

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	int32 NumberOfPellets = 15;
};
