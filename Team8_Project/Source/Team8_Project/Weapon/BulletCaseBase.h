// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletCaseBase.generated.h"

UCLASS()
class TEAM8_PROJECT_API ABulletCaseBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletCaseBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp
		, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void ShellDestroy();
public:	

protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* CasingMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USoundCue* ShellSound;

	FTimerHandle ShellTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShellEject")
	float ShellEjectionImpulse = 10.f;
};
