// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "../MyCharacter.h"
#include "Kismet/GameplayStatics.h" // Tracer
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "../Damageable.h"
#include "../MyPlayerController.h"
#include "WeaponCrosshairHUD.h"
#include "PlayerCombatOverlay.h"
#include "PlayerCombatComponent.h"



void AShotgunBase::BeginPlay()
{
	Super::BeginPlay();
}

int32 AShotgunBase::GetNumOfPellets()
{
	return NumberOfPellets;
}

void AShotgunBase::SetNumOfPellets(int32 NumOfPellets)
{
	NumberOfPellets = NumOfPellets;
}


void AShotgunBase::Fire(const FVector& HitTarget, float CurrentWeaponSpread)
{
	AWeaponBase::Fire(HitTarget, CurrentWeaponSpread);

	const USkeletalMeshSocket* MuzzleFlashSocket
		= GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		
		TMap<AActor*, uint32> HitMap;
		for (int32 i = 0; i < NumberOfPellets; i++)
		{
			//FVector End = TraceEndWithScatter(Start, HitTarget);

			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);
			AActor* HitActor = FireHit.GetActor();
			if (HitActor)
			{
				if (HitActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()) && OwnerPlayerController)
				{
					if (HitMap.Contains(HitActor))
					{
						HitMap[HitActor]++;
					}
					else
					{
						HitMap.Emplace(HitActor, 1);
					}
				}
			}

			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint,
					.5f,
					FMath::FRandRange(-.5f, .5f)
				);
			}
		}
		for (auto HitPair : HitMap)
		{
			if (HitPair.Key && OwnerPlayerController)
			{ // 펠릿 처리중 이미 죽으면 null이 되는 key에 대한 검증
				UGameplayStatics::ApplyDamage(
					HitPair.Key,
					HitScanDamage * HitPair.Value,
					OwnerPlayerController,
					this,
					UDamageType::StaticClass()
				);
			}
		}
		
	}
}
