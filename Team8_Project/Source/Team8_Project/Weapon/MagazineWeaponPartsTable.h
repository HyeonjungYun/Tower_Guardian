// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Inventory/InventoryType.h" 
#include "../BaseItem.h"
#include "WeaponPartsType.h"
#include "WeaponpartsActor.h"
#include "MagazineWeaponPartsTable.generated.h"

USTRUCT(BlueprintType)
struct FMagazineWeaponPartsTable : public FTableRowBase
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

    // Magazine Spec
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MagazineAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool isSpecial = false;
};

