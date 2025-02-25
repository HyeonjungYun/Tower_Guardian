// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_MeleeAttack.h"

#include "Components/ShapeComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Team8_Project/Damageable.h"

void UAN_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	
	UShapeComponent* Collision = nullptr;
	auto& Components = Owner->GetComponents();
	for (auto& Component : Components)
		if (Component->IsA<UShapeComponent>())
		{
			UShapeComponent* Temp = Cast<UShapeComponent>(Component);
			if (Temp->GetName() == CollisionName)
				Collision = Temp;
		}

	if (!Collision)
	{
		UE_LOG(LogTemp, Warning, TEXT("공격할 충돌체를 찾지 못했습니다."))
		return;
	}
	
	TArray<FOverlapResult> Overlaps;
	bool bOverlap = Owner->GetWorld()->ComponentOverlapMulti(Overlaps, Collision, Collision->GetComponentLocation(),
	                                                  Collision->GetComponentRotation());

	if (bOverlap)
	{
		TArray<AActor*> DamagedActors;	//중복 데미지 방지
		for (FOverlapResult& Overlap : Overlaps)
		{
			AActor* DamageActor = Overlap.GetActor();
			if (DamageActor && DamageActor != Owner && DamageActor->Implements<UDamageable>() &&
				!DamagedActors.Contains(DamageActor))
			{
				FDamageEvent DamageEvent;
				DamageActor->TakeDamage(Damage, DamageEvent, Owner->GetInstigatorController(), Owner);

				DamagedActors.Add(DamageActor);
			}
		}
	}
}
