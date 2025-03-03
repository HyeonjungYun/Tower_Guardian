// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API AProjectileRocket : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	AProjectileRocket();

	virtual void OnHit(UPrimitiveComponent* HitComp
		, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);


protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RocketMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
	float RocketExplosionOuterRadius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
	float RocketExplosionInnerRadius = 200.f;
};
