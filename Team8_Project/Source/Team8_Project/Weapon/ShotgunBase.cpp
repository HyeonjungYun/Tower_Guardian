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
		

		for (int32 i = 0; i < NumberOfPellets; i++)
		{
			FVector End = TraceEndWithScatter(Start, HitTarget);
		}
		
	}
}
