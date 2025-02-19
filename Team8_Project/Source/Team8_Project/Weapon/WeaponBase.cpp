// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase() :
WeaponState(EWeaponState::EWT_Equipped)
{
	WeaponType = EWeaponType::EWT_None;// 무기 없음초기화
	FItemType = "Weapon";


	// 스태틱 메시 컴포넌트 생성 및 설정
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	WeaponSkeletalMesh->SetupAttachment(Collision);
	
	// 모두 block pawn은 ignore
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 총기에서는 Static을 쓰지 않을꺼니까

	//StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponState == EWeaponState::EWT_Equipped)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 테스트 중 장비했을 때 콜리전이 발동되는 문제 제거
		// 추후 다시 인벤토리에 넣거나(필요하다면), 버릴 때 다시 활성화
	}
}

void AWeaponBase::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
}

FName AWeaponBase::GetItemType() const
{
	return FItemType;
}

EWeaponType AWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

int32 AWeaponBase::GetCurrrentWeaponAmmo() const
{
	return CurrentWeaponAmmo;
}

void AWeaponBase::SetCurrentWeaponAmmo(int32 _ammo)
{
}

void AWeaponBase::Fire()
{
	// 무기 발사
	if (FireAnimation)
	{
		// 스켈레탈 애니메이션을 반복하지 않고 재생
		WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}

void AWeaponBase::Reload()
{
}
