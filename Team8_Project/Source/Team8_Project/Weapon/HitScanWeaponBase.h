// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HitScanWeaponBase.generated.h"

#define TRACE_LENGTH 80000.f
/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API AHitScanWeaponBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Fire(const FVector& HitTarget, float CurrentWeaponSpread); // 히트스캔 무기 발사 재정의

	// 히트스캔 무기 분산 적용

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);

public:
	float GetDistanceToSphere();
	void SetDistanceToSphere(float _SDTS);

	float GetSphereRadius();
	void SetSphereRadius(float _SR);
protected:
	float HitScanDamage;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f; // 분산 거리

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 150.f; // 분산 범위

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;
};
