// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h" // Tracer
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "../Damageable.h"
#include "SampleDamagableActor.h"
#include "../MyPlayerController.h"
#include "WeaponCrosshairHUD.h"
#include "PlayerCombatOverlay.h"
#include "../MyCharacter.h"
#include "PlayerCombatComponent.h"
#include "WeaponBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	// Set Collision
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,ECollisionResponse::ECR_Block);
	
	ProjectileMovementComponent = 
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
	ProjectileMovementComponent->InitialSpeed = 30000.f;
	ProjectileMovementComponent->MaxSpeed = 30000.f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	

	AMyCharacter* OwnerPlayer = Cast<AMyCharacter>(GetOwner());

	if (OwnerPlayer)
	{
		ProjectileDamage =
		OwnerPlayer->GetCombatComponent()->GetEquippedWeapon()->GetWeaponDamage();
	}// 몬스터는 직접 투사체 데미지 설정

	// 총알 발사 후 총알 표현
	if (Tracer)
	{
		TracerComponent =
			UGameplayStatics::SpawnEmitterAttached(
				Tracer,
				CollisionBox,
				FName(),
				GetActorLocation(),
				GetActorRotation(),
				EAttachLocation::KeepWorldPosition
			);
	}
	SetLifeSpan(20.0f);// 충돌이 안되는 허공으로 날아가도 메모리 관리를 위해 제거
	// 레벨의 액터 숨기기 해제로 적용되는 것 확인
	
	// Onhit의 시그니처는 OnComponentHit의 정의에 나오는 구조체
	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	CollisionBox->IgnoreActorWhenMoving(Owner, true);

}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{

		AController* InstigatorController = nullptr;
		AActor* OwnerActor = GetOwner();
		if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
		{
			InstigatorController = OwnerPawn->GetController();
			UGameplayStatics::ApplyDamage(OtherActor, ProjectileDamage, InstigatorController, this, UDamageType::StaticClass());

			AMyPlayerController* PC = Cast<AMyPlayerController>(InstigatorController);
			if (PC)
			{
				AWeaponCrosshairHUD* WCHUD = Cast<AWeaponCrosshairHUD>(PC->GetHUD());
				if (WCHUD)
				{
					if (WCHUD->CombatOverlay)
					{
						WCHUD->CombatOverlay->PlayHitMarker();
					}
				}
			}
		}
	}

	Destroy();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckRebase

}

void AProjectileBase::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
	{
		//SpawnEmitterAtLocation 함수에서 자동 파괴 기능은 default 값이 true인 인자이다
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	// 소리 재생
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

