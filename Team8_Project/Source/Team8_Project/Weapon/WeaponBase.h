// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Inventory/ItemInterface.h"
#include "WeaponBase.generated.h"


class USphereComponent;
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

/*
    무기의 상태
*/
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    EWT_None UMETA(DisplayName = "None"),
    EWT_Dropped UMETA(DisplayName = "Dropped"),
    EWT_Equipped UMETA(DisplayName = "Equipped"),
    EWT_Inventory UMETA(DisplayName = "Inventory"),

    EWT_MAX UMETA(DisplayName = "DefaultMax") // 언리얼 열거형에서 열거형의 개수를 파악하기 위해 사용되는 기능

};

/**
 *  WeaponBase는 모든 무기의 Base 클래스가 됩니다.
 * 무기의 기능들을 담당하는 가상 함수들을 가지며 이는 상속받는 모든 무기들이
 * 재정의 해도록 구현될 것입니다.
 */
UCLASS()
class TEAM8_PROJECT_API AWeaponBase : public AActor, public IItemInterface
{
	GENERATED_BODY()

    /*
    
            IItemInterface    
    */
public:
    UFUNCTION()
    virtual void OnItemOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    virtual FName GetItemType() const;

public:

    // CombatComponent와 HUD에서 사용할 텍스처들

    UPROPERTY(EditAnywhere, Category = "Crosshairs")
    class UTexture2D* CrosshairsCenter;

    UPROPERTY(EditAnywhere, Category = "Crosshairs")
    UTexture2D* CrosshairsLeft;

    UPROPERTY(EditAnywhere, Category = "Crosshairs")
    UTexture2D* CrosshairsRight;

    UPROPERTY(EditAnywhere, Category = "Crosshairs")
    UTexture2D* CrosshairsTop;

    UPROPERTY(EditAnywhere, Category = "Crosshairs")
    UTexture2D* CrosshairsBottom;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
    EWeaponState WeaponState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 CurrentWeaponAmmo;

    // 총기용 USkeletalMeshComponent 추가
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
    USkeletalMeshComponent* WeaponSkeletalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 MaxWeaponAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties")
    class UAnimationAsset* FireAnimation;
   
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    //USceneComponent* Scene;
    
    // 충돌 컴포넌트 (플레이어 진입 범위 감지용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
    USphereComponent* AreaSphere;


    UPROPERTY(EditAnywhere)
    TSubclassOf<class ABulletCaseBase> BulletCaseClass;

    UPROPERTY()
    class AMyCharacter* PlayerCharacter;

    UPROPERTY()
    class AMyPlayerController* PlayerController;

    // 조준
    UPROPERTY(EditAnywhere)
    float ZoomedFOV = 30.f;

    UPROPERTY(EditAnywhere)
    float ZoomInterpSpeed = 20.f;

public:
    AWeaponBase();
    void SetWeaponState(EWeaponState CurWeaponState);
    UFUNCTION(BlueprintCallable)
    virtual void Fire(const FVector& HitTarget);

    float GetWeaponZoomFov() const;
    
    float GetWeaponZoomInterpSpeed() const;

    //virtual FName GetItemType() const override;

    EWeaponType GetWeaponType() const;

    int32 GetCurrrentWeaponAmmo() const;

    void SetCurrentWeaponAmmo(int32 _ammo);

    USphereComponent* GetAreaSphere() const { return AreaSphere; }
    USkeletalMeshComponent* GetWeaponMesh() const { return WeaponSkeletalMesh; }

    UFUNCTION(BlueprintCallable)
    virtual void Reload();
protected:
    virtual void BeginPlay() override;

    virtual void ActivateItem(AActor* Activator) override;


    /*
        탄약
    */
public:
    UFUNCTION()
    void SpendRound();
protected:
    UPROPERTY(EditAnywhere)
    int32 CurrentAmmo = 0;
    UPROPERTY(EditAnywhere)
    int32 MaxAmmo = 30;

    /*
    무기 버리기
    */

public:
    UFUNCTION()
    void Dropped();
};
