// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Inventory/InventoryType.h" 
#include "../BaseItem.h"
#include "WeaponPartsType.h"
#include "WeaponpartsActor.h"
#include "MuzzleWeaponPartsTable.generated.h"

USTRUCT(BlueprintType)
struct FMuzzleWeaponPartsTable : public FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemKey = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Weaponparts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemImage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<AWeaponpartsActor> WeaponPartsActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EWeaponPartsType WeaponPartsType = EWeaponPartsType::EWT_Grip;

    // Muzzle Spec
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float MuzzleSpread;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float MuzzleSoundAmount = 0.0f;
};

