// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_AddForce.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"


void UAN_AddForce::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APawn* Pawn = Cast<APawn>(MeshComp->GetOwner()))
		if (UFloatingPawnMovement* Movement = Cast<UFloatingPawnMovement>(Pawn->GetMovementComponent()))
			Movement->Velocity = Pawn->GetActorRotation().RotateVector(Force);
}
