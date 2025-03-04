// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ShootLaser.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team8_Project/Weapon/ProjectileBase.h"

void UAN_ShootLaser::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	USceneComponent* StartLocationScene = nullptr;

	auto& Comps = Owner->GetComponents();
	for (auto& Comp : Comps)
		if (USceneComponent* Scene = Cast<USceneComponent>(Comp))
			if (Scene->GetName() == StartLocationCompName)
				StartLocationScene = Scene;

	if (!StartLocationScene)
		return;

	APawn* Pawn = Cast<APawn>(Owner);
	if (!Pawn) return;

	AAIController* AIController = Pawn->GetController<AAIController>();
	if (!AIController) return;

	AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(ShootTargetBlackboardKeyName));
	if (!Target) return;	//조준 중 타겟을 잃어버림
	
	FRotator Dir = (Target->GetActorLocation() - StartLocationScene->GetComponentLocation()).Rotation();
	
	FActorSpawnParameters ProjectileSpawnParams;
	ProjectileSpawnParams.Owner = Owner;// 무기의 소유주에게 투사체의 소유권도 부여
	ProjectileSpawnParams.Instigator = Pawn;

	Owner->GetWorld()->SpawnActor<AProjectileBase>(LaserClass, StartLocationScene->GetComponentLocation(), Dir, ProjectileSpawnParams);
}
