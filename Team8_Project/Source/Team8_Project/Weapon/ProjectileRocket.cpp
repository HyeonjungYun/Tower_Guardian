// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APawn* FiringPawn =	GetInstigator();
	if (FiringPawn)
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this, // World Context Object 
				ProjectileDamage, // Damage => 무기의 속성값을 따라감
				10.f, // Minimum Damage(가장 먼곳에서 맞은 경우)
				GetActorLocation(), // 데미지 계신 시작위치
				RocketExplosionInnerRadius, // 데미지 계산 안쪽 구 반지름 길이
				RocketExplosionOuterRadius, // 데미지 계산 바깥 쪽 구 반지름 길이
				1.f, // 거리별 데미지 감소 수치(현재는 선형 감소)
			UDamageType::StaticClass(), // 데미지 타입 클래스
				TArray<AActor*>(), // 무시할 액터 넣기(현재는 없다는 의미)
				this,// 데미지 유발자(causer)
				FiringController // InstigatorController
			);
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
