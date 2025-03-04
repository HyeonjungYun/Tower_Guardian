// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RotateAndScaleWithName.h"

void UANS_RotateAndScaleWithName::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	TotalTime = TotalDuration;
	NowTime = 0;
}

void UANS_RotateAndScaleWithName::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	NowTime += FrameDeltaTime;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	USceneComponent* TargetComp = Cast<USceneComponent>(Owner->GetDefaultSubobjectByName(ComponentName));
	if (!TargetComp) return;

	TargetComp->AddLocalRotation(FRotator(0, RotateSpeed, 0) * FrameDeltaTime);
	
	float TimeRatio = NowTime / TotalTime;
	float Value = ScaleCurve.GetRichCurve()->Eval(TimeRatio);
	
	TargetComp->SetRelativeScale3D(FVector(Value));
}
