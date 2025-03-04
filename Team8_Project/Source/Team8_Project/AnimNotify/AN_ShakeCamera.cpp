// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ShakeCamera.h"

void UAN_ShakeCamera::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APlayerController* PlayerController = MeshComp->GetWorld()->GetFirstPlayerController())
		if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
			CameraManager->StartCameraShake(CameraShakeClass, Scale);
}
