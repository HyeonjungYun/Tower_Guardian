// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ProjectileWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API AProjectileWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AProjectileWeaponBase();
	virtual void Fire(const FVector& HitTarget) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AProjectileBase> ProjectileClass;
};
