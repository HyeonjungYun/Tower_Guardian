// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Inventory/ItemInterface.h"

#include "Components/SkeletalMeshComponent.h"
#include "WeaponType.h"
#include "WeaponpartsActor.h"
#include "Engine/DataTable.h"
#include "WeaponBase.generated.h"


class USphereComponent;
/*
*	무기의 종류를 결정할 무기 종류 열거형 클래스
*/

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName AmmoItemKey;

    // 총기용 USkeletalMeshComponent 추가
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
    USkeletalMeshComponent* WeaponSkeletalMesh;



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
    class AMyCharacter* OwnerPlayerCharacter;

    UPROPERTY()
    class AMyPlayerController* OwnerPlayerController;

    // 조준
    UPROPERTY(EditAnywhere)
    float ZoomedFOV = 30.f;

    UPROPERTY(EditAnywhere)
    float ZoomInterpSpeed = 20.f;

public:
    AWeaponBase();
    void SetWeaponState(EWeaponState CurWeaponState);
    UFUNCTION(BlueprintCallable)
    virtual void Fire(const FVector& HitTarget, float CurrentWeaponSpread);

    float GetWeaponZoomFov() const;
    
    float GetWeaponZoomInterpSpeed() const;

    //virtual FName GetItemType() const override;

    EWeaponType GetWeaponType() const;



    USphereComponent* GetAreaSphere() const { return AreaSphere; }
    USkeletalMeshComponent* GetWeaponMesh() const { return WeaponSkeletalMesh; }


protected:
    virtual void BeginPlay() override;

    virtual void ActivateItem(AActor* Activator) override;


    /*
        탄약
    */
public:
    UFUNCTION()
    void SpendRound();

    UFUNCTION(BlueprintCallable)
    int32 GetCurrrentWeaponAmmo() const;

    UFUNCTION(BlueprintCallable)
    void SetCurrentWeaponAmmo(int32 _Ammo);

    UFUNCTION(BlueprintCallable)
    int32 GetMaxWeaponAmmo() const;

    UFUNCTION(BlueprintCallable)
    void SetMaxWeaponAmmo(int32 _Ammo);

    bool IsWeaponMagEmpty();

    float GetWeaponDamage();
    void SetWeaponDamage(float NewDamage);
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    float WeaponDamage = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 CurrentWeaponAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
    int32 MaxWeaponAmmo;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    bool bIsWeaponAutomatic = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Fire")
    float WeaponFireRate = .15f;
    /*
        무기 버리기
    */

public:
    UFUNCTION()
    void OnWeaponEquipped(AMyCharacter* _OwnerPlayerCharacter
        ,AMyPlayerController* _OwnerPlayerController);
    UFUNCTION()
    void Dropped();

    /***
        재장전
    ***/
public:
    UFUNCTION(BlueprintCallable)
    virtual void Reload();
    UFUNCTION()
    float GetTimeToFinishReload();
    UFUNCTION()
    void SetTimeToFinishReload(float NewReloadTime);
protected:

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float TimeToFinishReload = 2.f;

    /***
        무기 모딩을 위한 세부 값들
    ***/
public:



public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponModify")
    bool bIsWeaponCanModify = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponModify|Optic")
    bool bIsCanModifyOptic = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponModify|Muzzle")
    bool bIsCanModifyMuzzle = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponModify|Magazine")
    bool bIsCanModifyMagazine = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponModify|ForeGrip")
    bool bIsCanModifyForeGrip = false;

    float WeaponVerticalSpreads = 0.0f;
    float WeapopnHorizontalSpreads = 0.0f;
    float WeaponCurrentSpreads = 0.0f;

    bool bIsInfiniteAmmo = false;
    

    // 부착물 구현
    // 모든 부착물 관련 데이터 가지고 있게하기
    // WeaponPartsComponent라는 별도의 컴포넌트로 기능 분리를 하면 더 좋겠으나.
    // 일단 만들자

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
    UDataTable* OpticPartsDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
    UDataTable* GripPartsDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
    UDataTable* MagazinePartsDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
    UDataTable* MuzzlePartsDataTable;

    TMap<FName, AWeaponpartsActor*> OpticActorContainer;
    TArray<struct FOpticWeaponPartsTable*> OpticWeaponpartsTableRows;

    TMap<FName, AWeaponpartsActor*> GripActorContainer;
    TArray<struct FGripWeaponPartsTable*> GripWeaponpartsTableRows;

    TMap<FName, AWeaponpartsActor*> MagzineActorContainer;
    TArray<struct FMagazineWeaponPartsTable*> MagzineWeaponpartsTableRows;

    TMap<FName, AWeaponpartsActor*> MuzzleActorContainer;
    TArray<struct FMuzzleWeaponPartsTable*> MuzzleWeaponpartsTableRows;

    void InitializeWeaponParts();

    // 현재 장착된 부착물 (각 부착물 유형별)
    AWeaponpartsActor* EquippedOpticPart = nullptr;
    AWeaponpartsActor* EquippedGripPart = nullptr;
    AWeaponpartsActor* EquippedMagazinePart = nullptr;
    AWeaponpartsActor* EquippedMuzzlePart = nullptr;

    UFUNCTION(BlueprintCallable)
    void DebugEnableWeaponParts(FName ItemKey);

    template <typename T>
    void SpawnAndStoreParts(const TArray<T*>& TableRows, TMap<FName, AWeaponpartsActor*>& PartContainer, FName SocketName)
    {

        UWorld* World = GetWorld();
        if (!World) return;

        for (T* Row : TableRows)
        {
            if (!Row || !Row->WeaponPartsActorClass) continue;

            // 1. 부착물 액터 생성 (숨김 상태로)
            AWeaponpartsActor* SpawnedPart = World->SpawnActor<AWeaponpartsActor>(Row->WeaponPartsActorClass);
            if (SpawnedPart)
            {
                // 2. SkeletalMesh에 부착
                SpawnedPart->AttachToComponent(WeaponSkeletalMesh,
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                    SocketName);

                // 3. 초기 숨김 처리 및 충돌 비활성화
                SpawnedPart->SetActorHiddenInGame(true);
                SpawnedPart->SetActorEnableCollision(false);
                UE_LOG(LogTemp, Warning, TEXT("Weapon Part Class: %s"), *SpawnedPart->GetClass()->GetName());


                // 4. TMap에 저장 (ItemKey를 키 값으로 사용)
                PartContainer.Add(Row->ItemKey, SpawnedPart);
            }
        }
    }
  };
