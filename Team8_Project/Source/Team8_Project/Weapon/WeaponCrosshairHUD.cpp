// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCrosshairHUD.h"
#include "Kismet/GameplayStatics.h"//crosshair
#include "PlayerCombatOverlay.h"
#include "HealthImageWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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

	float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairsSpread;

	if (HUDPackage.CrosshairsCenter)
	{
		FVector2D Spread(0.f, 0.f);
		DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread);
	}
	if (HUDPackage.CrosshairsLeft)
	{
		FVector2D Spread(-SpreadScaled, 0.f);
		DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread);
	}
	if (HUDPackage.CrosshairsRight)
	{
		FVector2D Spread(SpreadScaled, 0.f);
		DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread);
	}
	if (HUDPackage.CrosshairsTop)
	{
		FVector2D Spread(0.f, -SpreadScaled);// UV는 윗방향이 음의 수치이다..
		DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter,Spread);
	}
	if (HUDPackage.CrosshairsBottom)
	{
		FVector2D Spread(0.f, SpreadScaled);
		DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter,Spread);
	}
}

void AWeaponCrosshairHUD::SetHUDPackage(const FHUDPackage& Package)
{
	HUDPackage = Package;
}

void AWeaponCrosshairHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth/2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight/2.f) + Spread.Y
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

void AWeaponCrosshairHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCombatOverlay();
	//AddHealthOverlay();
	
	CallCreateHPSeg(InitHUDMaxHealth);

}

void AWeaponCrosshairHUD::AddCombatOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && CombatOverlayclass)
	{
		CombatOverlay = CreateWidget<UPlayerCombatOverlay>(PlayerController, CombatOverlayclass);
		CombatOverlay->AddToViewport();
	}
	if (CombatOverlay)
	{
		CombatOverlay->HUDMaxHealth = InitHUDMaxHealth;
	}
}

//void AWeaponCrosshairHUD::AddHealthOverlay()
//{
//	APlayerController* PlayerController = GetOwningPlayerController();
//
//	if (PlayerController && HealthWidgetclass)
//	{
//		HealthWidget = CreateWidget<UHealthImageWidget>(PlayerController, HealthWidgetclass);
//		HealthWidget->AddToViewport();
//	}
//}

void AWeaponCrosshairHUD::CallCreateHPSeg(float _InitHP)
{
	CombatOverlay->CreateHPSeg(_InitHP);
}

void AWeaponCrosshairHUD::SetReloadUIVisibility(bool bReloadVisible)
{
	if (bReloadVisible)
	{
		if (UTextBlock* ReloadText = Cast<UTextBlock>(CombatOverlay->GetWidgetFromName(TEXT("ReloadText"))))
		{
			ReloadText->SetVisibility(ESlateVisibility::Visible);
		}
		if (UProgressBar* ReloadPB = Cast<UProgressBar>(CombatOverlay->GetWidgetFromName(TEXT("ReloadPB"))))
		{
			ReloadPB->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (UTextBlock* ReloadText = Cast<UTextBlock>(CombatOverlay->GetWidgetFromName(TEXT("ReloadText"))))
		{
			ReloadText->SetVisibility(ESlateVisibility::Hidden);
		}
		if (UProgressBar* ReloadPB = Cast<UProgressBar>(CombatOverlay->GetWidgetFromName(TEXT("ReloadPB"))))
		{
			ReloadPB->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void AWeaponCrosshairHUD::UpdateReloadUIProgress(float _RemainTime, float _Duration)
{
	if (_RemainTime > 0.0f && _Duration>0.0f)
	{
		if (UProgressBar* ReloadPB = Cast<UProgressBar>(CombatOverlay->GetWidgetFromName(TEXT("ReloadPB"))))
		{
			float ReloadingPercentage = _RemainTime / _Duration;
			ReloadPB->SetPercent(ReloadingPercentage);
		}
	}
}
