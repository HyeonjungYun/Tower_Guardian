// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeaponBase.h"
#include "ProjectileBase.h"
#include "Engine/SkeletalMeshSocket.h"

AProjectileWeaponBase::AProjectileWeaponBase()
{

}

void AProjectileWeaponBase::Fire(const FVector& HitTarget, float CurrentWeaponSpread)
{
	Super::Fire(HitTarget, CurrentWeaponSpread);
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	//투사체 발사무기의 Fire 함수 재정의 =>
	// 투사체(ProjectileBase) SpawnActor하기
	// 이미 SKmesh에 있는 MuzzleFlash 소켓 사용

	const USkeletalMeshSocket* MuzzleFlashSocket =
	GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));

	if (MuzzleFlashSocket)
	{
		// mesh로 부터 발사 위치 소켓의 Transform 받기
		FTransform SocketTransform=
		MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		// 발사 위치에 대한 정보, 벡터 A-B => B에서 A를 향하는 크기를 포함한 방향 벡터
		FVector ToTargetPos = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTargetPos.Rotation();
		if (ProjectileClass && InstigatorPawn)
		{
			FActorSpawnParameters ProjectileSpawnParams;
			ProjectileSpawnParams.Owner = GetOwner();// 무기의 소유주에게 투사체의 소유권도 부여
			ProjectileSpawnParams.Instigator = InstigatorPawn;

			if (bIsWeaponHasSpread)
			{
				ReturnFinalSpread(DefaultSpread, FinalSpread);
				TargetRotation.Pitch += FMath::RandRange(-FinalSpread, FinalSpread);
				TargetRotation.Yaw += FMath::RandRange(-FinalSpread, FinalSpread);
			}

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AProjectileBase>(ProjectileClass,
					SocketTransform.GetLocation(),
					TargetRotation,
					ProjectileSpawnParams);
			}
		}
	}

}
