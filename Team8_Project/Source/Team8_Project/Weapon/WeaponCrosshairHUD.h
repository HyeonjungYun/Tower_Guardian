// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WeaponCrosshairHUD.generated.h"

// 총마다 달라질 크로스헤어를 위한 구조체
USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairsSpread;
};

/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API AWeaponCrosshairHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	void SetHUDPackage(const FHUDPackage& Package);
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);

	virtual void BeginPlay() override;
	void AddCombatOverlay();
	//void AddHealthOverlay();
	void CallCreateHPSeg(float _InitHP);
	void SetReloadUIVisibility(bool bReloadVisible);
	void UpdateReloadUIProgress(float _RemainTime, float _Duration);
protected:
	FHUDPackage HUDPackage;
	float CrosshairSpreadMax = 16.f;


public:
	UPROPERTY(EditAnywhere, Category = "CombatOverlay")
	TSubclassOf<class UUserWidget> CombatOverlayclass;

	class UPlayerCombatOverlay* CombatOverlay;

	UPROPERTY(EditAnywhere, Category = "CombatOverlay")
	TSubclassOf<class UUserWidget> HealthWidgetclass;

	class UHealthImageWidget* HealthWidget;

	UPROPERTY(EditAnywhere)
	float InitHUDMaxHealth;
};
