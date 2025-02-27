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

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void CreateHPSeg(float Max_);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateHPSeg(float PlayerCurrentHealth,float PlayerMaxHealth);
};
