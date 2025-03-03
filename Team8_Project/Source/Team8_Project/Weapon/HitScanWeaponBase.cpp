// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeaponBase.h"
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
void AHitScanWeaponBase::BeginPlay()

{
	Super::BeginPlay();
	AMyCharacter* OwnerPlayer = Cast<AMyCharacter>(GetOwner());

	HitScanDamage = GetWeaponDamage();
}

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
				
				// 추후 데미지 계산
				AActor* OtherActor = FireHit.GetActor();
				// 히트스캔
				if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
				{
					AController* InstigatorController = nullptr;
					AActor* OwnerActor = GetOwner();
					if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
					{
						InstigatorController = OwnerPawn->GetController();
						UGameplayStatics::ApplyDamage(OtherActor, HitScanDamage, InstigatorController, this, UDamageType::StaticClass());
					
						AMyPlayerController* PC = Cast<AMyPlayerController>(InstigatorController);
						if (PC)
						{
							AWeaponCrosshairHUD* WCHUD = Cast<AWeaponCrosshairHUD>(PC->GetHUD());
							if (WCHUD && WCHUD->CombatOverlay)
							{
								WCHUD->CombatOverlay->PlayHitMarker();
							}
						}
					}
				}
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


