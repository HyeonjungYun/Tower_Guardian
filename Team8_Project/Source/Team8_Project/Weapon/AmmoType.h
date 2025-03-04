// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
    EAT_None UMETA(DisplayName = "None"),
    EAT_Pistol UMETA(DisplayName = "Pistol Ammo"),
    EAT_Rifle UMETA(DisplayName = "Rifle Ammo"),
    EAT_Shotgun UMETA(DisplayName = "Shotgun Shells"),
    EAT_Rocket UMETA(DisplayName = "Rocket"),
    EAT_Sniper UMETA(DisplayName = "Sniper Ammo")
};
/**
 * 
 */
class TEAM8_PROJECT_API AmmoType
{
public:
	AmmoType();
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    EAmmoType GetAmmoTypeForWeapon(EWeaponType WeaponType);
};
