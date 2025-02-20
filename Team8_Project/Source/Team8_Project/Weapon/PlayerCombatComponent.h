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
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void EquipWeapon(AWeaponBase* WeaponToEquip);

	void FireButtonPressed(bool bPressed);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
protected:
	class AMyCharacter* PlayerCharacter;
	AWeaponBase* EquippedWeapon;
	bool bIsAiming = false;
	bool bFireButtonPressed = false;
	
	FVector HitTargetPos;
};
