// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCombatComponent.generated.h"

class AWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_PROJECT_API UPlayerCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerCombatComponent();
	friend class AMyCharacter;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void EquipWeapon(AWeaponBase* WeaponToEquip);

	void FireButtonPressed(bool bPressed);
protected:
	class AMyCharacter* PlayerCharacter;
	AWeaponBase* EquippedWeapon;
	bool bIsAiming = false;
	bool bFireButtonPressed = false;
};
