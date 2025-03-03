// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCombatOverlay.generated.h"

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API UPlayerCombatOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Health")
	float HUDCurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Health")

	float HUDMaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Ammo")
	int32 HUDCurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Ammo")
	int32 HUDMaxAmmo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* HealthBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_HealthMain;


	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//class UBorder*AmmoBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HB_Ammo_Main;

	UPROPERTY(VisibleAnywhere, meta =(BindWidget))
	class UTextBlock* WeaponAmmoAmount;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* InventoryAmmoAmount;
	
	class UUserWidget* HitMarker;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void CreateHPSeg(float Max_);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateHPSeg(float PlayerCurrentHealth,float PlayerMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void CreateAmmoSeg(int32 MaxAmmo);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateAmmoSeg(int32 PlayerCurrentAmmo);

	UFUNCTION(BlueprintCallable)
	void PlayHitMarker();
};
