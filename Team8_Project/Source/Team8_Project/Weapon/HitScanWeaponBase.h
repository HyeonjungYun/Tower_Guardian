// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HitScanWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API AHitScanWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Fire(const FVector& HitTarget); // 히트스캔 무기 발사 재정의

protected:
	float HitScanDamage;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;
};
