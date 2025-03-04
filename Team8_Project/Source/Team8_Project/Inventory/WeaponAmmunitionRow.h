// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Inventory/InventoryType.h" 
#include "../Weapon/WeaponBase.h"
#include "WeaponAmmunitionRow.generated.h"


//UENUM(BlueprintType)
//enum class EWeaponType : uint8
//{
//    EWT_None UMETA(DisplayName = "None"),
//    EWT_Pistol UMETA(DisplayName = "Pistol"),
//    EWT_Rifle UMETA(DisplayName = "Rifle"),
//    EWT_Shotgun UMETA(DisplayName = "Shotgun"),
//    EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
//    EWT_Sniper UMETA(DisplayName = "BoltAction")
//
//};


USTRUCT(BlueprintType)
struct FWeaponAmmunitionRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemKey = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Ammo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EWeaponType WeaponType = EWeaponType::EWT_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemImage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ItemSalePrice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ItemPurchasePrice;
};
