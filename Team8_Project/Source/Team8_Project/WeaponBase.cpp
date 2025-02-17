// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	WeaponType = EWeaponType::EWT_None;// 무기 없음초기화
	FItemType = "Weapon";
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
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
}

void AWeaponBase::Reload()
{
}
