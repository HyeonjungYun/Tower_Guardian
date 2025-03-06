// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponCrosshairHUD.h"
#include "WeaponType.h"
#include "WeaponPartsType.h"
#include "PlayerCombatComponent.generated.h"

class AWeaponBase;

#define TRACE_LENGTH_FOR_CROSSHAIR 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_PROJECT_API UPlayerCombatComponent : public UActorComponent
{ 
	GENERATED_BODY()

public:	

	UPlayerCombatComponent();
	friend class AMyCharacter;

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime); // 총 타입마다 달라질 크로스헤어조절
	UFUNCTION(BlueprintCallable)
	void UpdateHealth();
	void SetHUDHealth(float CurrentHealth, float MaxHealth);
	AWeaponBase* GetEquippedWeapon();

	float GetCurrentPlayerHealth();
	void SetCurrentPlayerHealth(float _HP);
	float GetMaxPlayerHealth();
	void SetMaxPlayerHealth(float _HP);

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void EquipWeapon(AWeaponBase* WeaponToEquip);

	void FireButtonPressed(bool bPressed);

	void SetAiming(bool _bIsAiming);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void InterpFOV(float DeltaTime);


protected:
	class AMyCharacter* PlayerCharacter;
	class AMyPlayerController* PlayerController;
	class AWeaponCrosshairHUD* PlayerCrosshairHUD;
	class UInventoryComponent* PlayerInventory;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	AWeaponBase* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bFireButtonPressed = false;
	
	FVector HitTargetPos;
	
	// 이동, 점프 동작시 조준선 벌어짐의 영향을 줄 요인들
	FHUDPackage HUDPackage;
	UPROPERTY(EditAnywhere)
	float CrosshairVelocityFactor;
	
	UPROPERTY(EditAnywhere)
	float CrosshairinAirFactor;
	
	UPROPERTY(EditAnywhere)
	float CrosshairAimFactor;

	UPROPERTY(EditAnywhere)
	float CrosshairBaseFactor = 0.5f;

	UPROPERTY(EditAnywhere)
	float CrosshairShootingFactor = 0.f;

	// 조준
	float DefaultFOV; // 기본 조준
	
	float CurrentFOV;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFov = 30.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.f;


	// Status
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PlayerCurrentHealth = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurWeaponAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxWeaponAmmo;
	bool bInitHpSeg = false;


public:
	// 테스트용 더미 코드 인벤토리
	TMap<EWeaponType, int32> CarriedAmmoMap;
	// 현재 들고있는 무기에 맞는 탄이 더미 인벤토리에 몇개있는지 Map에서 빼올 변수
	int32 CurWeaponInvenAmmo;
	/*
		발사, 발사 속도 조절
	*/


	// 분산도
	float CurrentWeaponSpread = 0.0f;
public:
	
	bool bIsCanFireinRate = true;// 무기 발사속도에따라 
	void StartFireTimer();
	void FireTimerFinished();
	bool WeaponCanFire();
	void ComponentFire();

protected:
	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay;


	/***
			재 장 전
	***/

public:

	void UpdateReloadUI();

	UFUNCTION()
	void ApplyReloadUI();
	UFUNCTION()
	void DeApplyReloadUI();

	void StartWeaponReload();
	
	void OnFinishWeaponReload();

	// 무한탄 아이템
	void OnInfiniteAmmoStart(float InfiniteTimerAmount);

	void OnInfiniteAmmoEnd();

	bool bIsInfiniteAmmo = false;
protected:
	bool bIsReloading = false;
	
	FTimerHandle FReloadTimerHandle;

	FTimerHandle FInfiniteAmmo;
public:
	bool IsPlayerDead();

	/***
		무기 파츠 가능 여부 관리
	***/

public:
	// 플레이어가 가지고 있을 웨폰 파츠들
	// 해당 파츠에 대한 사용 여부를 가진다.
	TMap<FName, bool> AvailableWeaponParts;
	EWeaponPartsType GetWeaponPartsTypefromFName(FName ItemKey);
	FName GetItemKeyFromWeaponPartsType(EWeaponPartsType WTP);

	UFUNCTION(BlueprintCallable)
	void UnlockWeaponParts(FName ItemKey);
	// 무기 부착물 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parts")
	UDataTable* WeaponPartsDataTable;
	// 부착물 정보 Row를 보관하기
	TArray<struct FWeaponPartsTableRow*> WeaponpartsTableRows;

	void InitializeWeaponParts();
	bool IsWeaponPartsAvailable(FName ItemKey);


	void OnCombatComponentModifyingUI();
	void OnCombatComponentModArrowUp() {};
	void OnCombatComponentModArrowDown() {};
	void OnCombatComponentModArrowLeft() {};
	void OnCombatComponentModArrowRight() {};

	bool bIsWeaponNowModding = false;
};
