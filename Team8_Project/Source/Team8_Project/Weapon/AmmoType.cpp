// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoType.h"

AmmoType::AmmoType()
{
}

EAmmoType AmmoType::GetAmmoTypeForWeapon(EWeaponType WeaponType)
{
    switch (WeaponType)
    {
    case EWeaponType::EWT_Pistol: return EAmmoType::EAT_Pistol;
    case EWeaponType::EWT_Rifle: return EAmmoType::EAT_Rifle;
    case EWeaponType::EWT_Shotgun: return EAmmoType::EAT_Shotgun;
    case EWeaponType::EWT_RocketLauncher: return EAmmoType::EAT_Rocket;
    case EWeaponType::EWT_Sniper: return EAmmoType::EAT_Sniper;
    default: return EAmmoType::EAT_None;
    }
}