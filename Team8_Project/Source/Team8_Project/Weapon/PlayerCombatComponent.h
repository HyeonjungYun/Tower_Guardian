// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void EquipWeapon(AWeaponBase* WeaponToEquip);

	void FireButtonPressed(bool bPressed);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void InterpFOV(float DeltaTime);
protected:
	class AMyCharacter* PlayerCharacter;
	class AMyPlayerController* PlayerController;
	class AWeaponCrosshairHUD* PlayerCrosshairHUD;
	AWeaponBase* EquippedWeapon;
	bool bIsAiming = false;
	bool bFireButtonPressed = false;
	
	FVector HitTargetPos;
	
	// 이동, 점프 동작시 조준선 벌어짐의 영향을 줄 요인들
	float CrosshairVelocityFactor;
	float CrosshairinAirFactor;

	// 조준
	float DefaultFOV; // 기본 조준
	
	float CurrentFOV;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFov = 30.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.f;
};
