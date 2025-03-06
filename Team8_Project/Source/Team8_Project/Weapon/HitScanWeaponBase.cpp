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
#include "Kismet/KismetMathLibrary.h"
#include "WeaponType.h"

#include "DrawDebugHelpers.h"

void AHitScanWeaponBase::BeginPlay()

{
	Super::BeginPlay();
	AMyCharacter* OwnerPlayer = Cast<AMyCharacter>(GetOwner());

	HitScanDamage = GetWeaponDamage();
}

void AHitScanWeaponBase::Fire(const FVector& HitTarget, float CurrentWeaponSpread)
{
	Super::Fire(HitTarget,CurrentWeaponSpread);

	const USkeletalMeshSocket* MuzzleFlashSocket
		= GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		//FVector End = Start + (HitTarget - Start) * 1.25f;
	
		FHitResult FireHit;
		WeaponTraceHit(Start,HitTarget,FireHit);

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
				FireHit.ImpactPoint
			);
		}
	}
}

FVector AHitScanWeaponBase::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	// 분산도 적용하기
	ReturnFinalSpread(DefaultSpread, FinalSpread);
	float AdjustedSphereRadius = SphereRadius * FinalSpread;
	if (WeaponType == EWeaponType::EWT_Shotgun)
	{
		AdjustedSphereRadius *= 2.f;
	}
	FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, AdjustedSphereRadius);
	FVector EndLoc = SphereCenter + RandVec;
	FVector ToEndLoc = EndLoc - TraceStart;

	// 분산도 디버깅 코드
	//DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, true);
	//DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, true);
	//DrawDebugLine(
	//	GetWorld(),
	//	TraceStart,
	//	FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()), // 오버플로우 방지
	//	FColor::Cyan,
	//	true);

	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());  // 오버플로우 방지
}

void AHitScanWeaponBase::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World)
	{
		// 분산을 사용하는 무기인가?
		FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart) * 1.25f;


		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(GetOwner());
		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility,
			TraceParams
		);
		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				TraceStart,
				FRotator::ZeroRotator,
				true
			);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}

float AHitScanWeaponBase::GetDistanceToSphere()
{
	return DistanceToSphere;
}

void AHitScanWeaponBase::SetDistanceToSphere(float _SDTS)
{
	DistanceToSphere = _SDTS;
}

float AHitScanWeaponBase::GetSphereRadius()
{
	return SphereRadius;
}

void AHitScanWeaponBase::SetSphereRadius(float _SR)
{
	SphereRadius = _SR;
}


