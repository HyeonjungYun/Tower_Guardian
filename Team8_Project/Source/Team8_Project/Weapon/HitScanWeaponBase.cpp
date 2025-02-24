// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../MyCharacter.h"
#include "Kismet/GameplayStatics.h" // Tracer
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

void AHitScanWeaponBase::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	const USkeletalMeshSocket* MuzzleFlashSocket
		= GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;
	
		FHitResult FireHit;
		UWorld* World = GetWorld();
		if (World)
		{
			World->LineTraceSingleByChannel(
			FireHit,
				Start,
				End,
				ECollisionChannel::ECC_Visibility);
			
			FVector BeamEnd = End;
			if (BeamParticles)
			{

				// 무기 발사 후 후연 생성
				UParticleSystemComponent* Beam =
					UGameplayStatics::SpawnEmitterAtLocation(World,
						BeamParticles,
						SocketTransform);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"),BeamEnd);
				}

			}

			if (FireHit.bBlockingHit)
			{
				// 히트스캔
				// 추후 데미지 계산



				// 피격 이펙트 생성
				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						World,
						ImpactParticles,
						FireHit.ImpactPoint,
						FireHit.ImpactNormal.Rotation()
					);
				}
			}
		}
	}
}
