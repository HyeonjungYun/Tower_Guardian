// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletCaseBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h" // Tracer
ABulletCaseBase::ABulletCaseBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletCaseMesh"));
	SetRootComponent(CasingMesh);

	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);

	CasingMesh->SetNotifyRigidBodyCollision(true); // Physic를 통해 발생시키는 hit이벤트사용을 위해 필요

}

void ABulletCaseBase::BeginPlay()
{
	Super::BeginPlay();
	CasingMesh->OnComponentHit.AddDynamic(this, &ABulletCaseBase::OnHit);
	CasingMesh->AddImpulse(GetActorForwardVector()* ShellEjectionImpulse);


	GetWorldTimerManager().SetTimer(
		ShellTimerHandle, this, &ABulletCaseBase::ShellDestroy, 3.0f, false);
}

void ABulletCaseBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
}

void ABulletCaseBase::ShellDestroy()
{
	GetWorldTimerManager().ClearTimer(ShellTimerHandle);
	Destroy();
}

