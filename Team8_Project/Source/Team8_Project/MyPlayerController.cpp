#include "MyPlayerController.h"
#include "Team8_Project/GameState/CH8_GameState.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponCrosshairHUD.h"
#include "Weapon/PlayerCombatOverlay.h"
#include "Components/TextBlock.h"

AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr),
	LookAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	AimingAction(nullptr),
	ShopOpenAction(nullptr),
	WeaponModifyingAction(nullptr)
{
	//WeaponCrosshairHUD = Cast<AWeaponCrosshairHUD>(GetHUD());
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				LocalPlayerSubsystem->AddMappingContext(InputMappingContext,0);
			}
		}
	}

	if (ACH8_GameState* CH8GameState = GetWorld() ? GetWorld()->GetGameState<ACH8_GameState>() : nullptr)
	{
		CH8GameState->UpdateHUD();
	}

	WeaponCrosshairHUD = Cast<AWeaponCrosshairHUD>(GetHUD());
}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMyPlayerController::DisplayHUD()
{
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}

void AMyPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	WeaponCrosshairHUD = WeaponCrosshairHUD == nullptr ? Cast<AWeaponCrosshairHUD>(GetHUD()) : WeaponCrosshairHUD;
	bool bHUDValid = WeaponCrosshairHUD &&
		WeaponCrosshairHUD->CombatOverlay;
	if (bHUDValid)
	{
		WeaponCrosshairHUD->CombatOverlay->UpdateAmmoSeg(Ammo);

		FString WeaponAmmoText = FString::Printf(TEXT("%d"), Ammo);
		WeaponCrosshairHUD->CombatOverlay->WeaponAmmoAmount->SetText(FText::FromString(WeaponAmmoText));
	}
}

void AMyPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	WeaponCrosshairHUD = WeaponCrosshairHUD == nullptr ? Cast<AWeaponCrosshairHUD>(GetHUD()) : WeaponCrosshairHUD;
	bool bHUDValid = WeaponCrosshairHUD &&
		WeaponCrosshairHUD->CombatOverlay;
	if (bHUDValid)
	{
		FString InventoryAmmoText = FString::Printf(TEXT("%d"), Ammo);
		WeaponCrosshairHUD->CombatOverlay->InventoryAmmoAmount->SetText(FText::FromString(InventoryAmmoText));
	}
}

void AMyPlayerController::InitHUDWeaponAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	WeaponCrosshairHUD = WeaponCrosshairHUD == nullptr ? Cast<AWeaponCrosshairHUD>(GetHUD()) : WeaponCrosshairHUD;
	bool bHUDValid = WeaponCrosshairHUD &&
		WeaponCrosshairHUD->CombatOverlay;
	if (bHUDValid)
	{
		WeaponCrosshairHUD->CombatOverlay->CreateAmmoSeg(MaxAmmo);
		WeaponCrosshairHUD->CombatOverlay->UpdateAmmoSeg(CurrentAmmo);
	}
}

AWeaponCrosshairHUD* AMyPlayerController::GetWeaponCrosshairHUD()
{
	return WeaponCrosshairHUD;
}

