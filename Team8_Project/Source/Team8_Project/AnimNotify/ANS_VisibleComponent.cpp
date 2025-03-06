// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_VisibleComponent.h"

void UANS_VisibleComponent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	auto& Components = MeshComp->GetOwner()->GetComponents();
	for (auto& Comp : Components)
		if (Comp->GetName() == ComponentName)
			if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Comp))
				Mesh->SetVisibility(true);
}

void UANS_VisibleComponent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	auto& Components = MeshComp->GetOwner()->GetComponents();
	for (auto& Comp : Components)
		if (Comp->GetName() == ComponentName)
			if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Comp))
				Mesh->SetVisibility(false);
}
