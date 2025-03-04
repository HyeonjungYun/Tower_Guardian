// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ShootLaser.generated.h"

class AProjectileBase;
/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API UAN_ShootLaser : public UAnimNotify
{
	GENERATED_BODY()

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FName StartLocationCompName;

	UPROPERTY(EditAnywhere)
	FName ShootTargetBlackboardKeyName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> LaserClass;
};
