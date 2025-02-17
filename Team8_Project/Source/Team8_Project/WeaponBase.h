// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "WeaponBase.generated.h"



/*
*	무기의 종류를 결정할 무기 종류 열거형 클래스
*/
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_None UMETA(DisplayName="None"),
    EWT_Pistol UMETA(DisplayName = "Pistol"),
    EWT_Rifle UMETA(DisplayName = "Rifle"),
    EWT_Shotgun UMETA(DisplayName = "Shotgun"),
    EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
    EWT_Sniper UMETA(DisplayName = "BoltAction")

};
/**
 *  WeaponBase는 모든 무기의 Base 클래스가 됩니다.
 * 무기의 기능들을 담당하는 가상 함수들을 가지며 이는 상속받는 모든 무기들이
 * 재정의 해도록 구현될 것입니다.
 */
UCLASS()
class TEAM8_PROJECT_API AWeaponBase : public ABaseItem
{
	GENERATED_BODY()
	
public:
   
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 CurrentWeaponAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 MaxWeaponAmmo;

public:
    AWeaponBase();

protected:
    virtual void BeginPlay() override;

    virtual void ActivateItem(AActor* Activator) override;

    virtual FName GetItemType() const override;

    EWeaponType GetWeaponType() const ;

    int32 GetCurrrentWeaponAmmo() const;

    void SetCurrentWeaponAmmo(int32 _ammo);

    virtual void Fire();
    virtual void Reload();
};
