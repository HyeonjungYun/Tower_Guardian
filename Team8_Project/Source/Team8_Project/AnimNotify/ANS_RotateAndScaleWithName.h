// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotateAndScaleWithName.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API UANS_RotateAndScaleWithName : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FName ComponentName;
	
	UPROPERTY(EditAnywhere)
	float RotateSpeed;

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve ScaleCurve;

private:
	float TotalTime;
	float NowTime;
};
