// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatOverlay.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"

void UPlayerCombatOverlay::PlayHitMarker()
{
	HitMarker = Cast<UUserWidget>(GetWidgetFromName(FName("WBP_HitMarker")));
	if (HitMarker)
	{
		HitMarker->SetVisibility(ESlateVisibility::Visible);
		UFunction* FadeOutAnim = FindFunction(FName("FuncPlayHitMarkerAnim"));
		if (FadeOutAnim)
		{
			ProcessEvent(FadeOutAnim, nullptr);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("히트마커 없음"));
	}
}
