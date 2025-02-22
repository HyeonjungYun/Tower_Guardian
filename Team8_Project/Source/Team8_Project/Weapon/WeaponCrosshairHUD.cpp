// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCrosshairHUD.h"
#include "Kismet/GameplayStatics.h"//crosshair
void AWeaponCrosshairHUD::DrawHUD()
{
	Super::DrawHUD();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerCombatComponent::TraceUnderCrosshairs에서 PlayerConroller nullptr"));
		return;
	}

	FVector2D ViewportSize = FVector2D::ZeroVector;
	if (PC->GetLocalPlayer()->ViewportClient)
	{
		PC->GetLocalPlayer()->ViewportClient->GetViewportSize(ViewportSize);
	}

	const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

	if (HUDPackage.CrosshairsCenter)
	{
		DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter);
	}
	if (HUDPackage.CrosshairsLeft)
	{
		DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter);
	}
	if (HUDPackage.CrosshairsRight)
	{
		DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter);
	}
	if (HUDPackage.CrosshairsTop)
	{
		DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter);
	}
	if (HUDPackage.CrosshairsBottom)
	{
		DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter);
	}
}

void AWeaponCrosshairHUD::SetHUDPackage(const FHUDPackage& Package)
{
	HUDPackage = Package;
}

void AWeaponCrosshairHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth/2.f),
		ViewportCenter.Y - (TextureHeight/2.f)
	);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,1.f,FLinearColor::White
	);

}
