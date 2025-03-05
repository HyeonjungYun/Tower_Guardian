// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DamageSelf.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Team8_Project/Damageable.h"

void UAN_DamageSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APawn* Owner = Cast<APawn>(MeshComp->GetOwner()))
		if (Owner->Implements<UDamageable>())
			Owner->TakeDamage(Damage, FDamageEvent(), Owner->GetController(), Owner);
}
