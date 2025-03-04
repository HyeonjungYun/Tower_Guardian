#pragma once
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_None UMETA(DisplayName = "None"),
    EWT_Pistol UMETA(DisplayName = "Pistol"),
    EWT_Rifle UMETA(DisplayName = "Rifle"),
    EWT_Shotgun UMETA(DisplayName = "Shotgun"),
    EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
    EWT_Sniper UMETA(DisplayName = "BoltAction")
};

