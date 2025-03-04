#pragma once
UENUM(BlueprintType)
enum class EWeaponPartsType : uint8
{
    EWT_None UMETA(DisplayName = "None"),
    EWT_Optic UMETA(DisplayName = "Optic"),
    EWT_Muzzle UMETA(DisplayName = "Muzzle"),
    EWT_Grip UMETA(DisplayName = "Grip"),
    EWT_Magazine UMETA(DisplayName = "Magazine")
};

