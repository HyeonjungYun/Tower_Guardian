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
// Component에 Selected Table 변수하나 만들기 
// ADD를 테이블별로 나누기 작업을 하자  , 
// 서브시스템ㅇ만 ADD를 나누고 컴포넌트에서 호출하는걸로
// 외부나 인터페이스에서 수정할건 없다 add호출해도 Component에서 걸러주면되니깐
// 그리고 서브시스템에 Get을 가져와서 외부에서 검색보다는 Search로 열어주자
// ItemType과 Name을 넣어줘야겠다 