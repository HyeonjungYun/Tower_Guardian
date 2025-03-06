// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"
#include "WeaponBase.h"
#include "../MyCharacter.h"
#include "Engine/SkeletalMeshSocket.h"// 소켓
#include "Kismet/GameplayStatics.h"//crosshair
#include "DrawDebugHelpers.h"
#include "../MyPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "PlayerCombatOverlay.h"

#include "TimerManager.h"

#include "Engine/DataTable.h"
#include "WeaponPartsTableRow.h"

#include "../Inventory/InventoryComponent.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true; //디버그 틱
	PlayerCharacter = nullptr;
	EquippedWeapon = nullptr;


	CarriedAmmoMap.Add(EWeaponType::EWT_Sniper,30);
	CarriedAmmoMap.Add(EWeaponType::EWT_Rifle, 100);
	CarriedAmmoMap.Add(EWeaponType::EWT_RocketLauncher, 5);
	CarriedAmmoMap.Add(EWeaponType::EWT_Shotgun,20);


}

void UPlayerCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
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

	// 매개변수에게 뷰포트 값이붙음(OUT 매크로)

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);// 스크린 화면의 중앙

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld=
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{// 뷰포트 정중앙에서 World의 한지점까지 deproject한 것이 성공했나
		FVector Start = CrosshairWorldPosition;

		// 카메라랑 플레이어 사이에 있는 무언가를 조준 카메라가 무시하기 위한 코드
		if (PlayerCharacter)
		{
			float DistanceToCharcater = (PlayerCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharcater + 100.f);
			
		}

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH_FOR_CROSSHAIR; // 시작점 + 방향*거리

		// 캐릭터나 서브클래스를 무시하기 위해 Query Params 설정
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());  // 현재 플레이어(또는 해당 Actor) 무시

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			QueryParams
		);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End; // 거리안에 닿는것이 없다면 마지막을
			HitTargetPos = End;
		}
		else
		{
			HitTargetPos = TraceHitResult.ImpactPoint;
		}

	}
}

void UPlayerCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (PlayerCharacter == nullptr || PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player 또는 Controller없음"));
		return;
	}
	PlayerController = PlayerController == nullptr ?
		Cast<AMyPlayerController>(PlayerCharacter->Controller) : PlayerController ;
	if (PlayerController)
	{
		PlayerCrosshairHUD = PlayerCrosshairHUD == nullptr ?
			Cast<AWeaponCrosshairHUD>(PlayerController->GetHUD()) : PlayerCrosshairHUD;
		if (PlayerCrosshairHUD)
		{

			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
			}
			else
			{// 무기 미장착시
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}

			// 조준선 퍼짐 결정하기
			// 걷기 상태의 퍼짐
			// 현재 걷기상태의 임의 값이 600이기 때문에 0~600을 0~1로 맵핑
			FVector2D WalkSpeedRange(0.f, PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = PlayerCharacter->GetVelocity();
			Velocity.Z = 0.f;
			CrosshairVelocityFactor =
				FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
			
			// 현재 속도에 따른 0~1 사이의 범위가 정해져 이것이
			// HUD에 넘어갈구조체의 조준선 퍼짐 값으로 결정됨
			
			if (PlayerCharacter->GetCharacterMovement()->IsFalling())
			{
				CrosshairinAirFactor
					= FMath::FInterpTo(CrosshairinAirFactor,2.25f,DeltaTime,2.25f);
			}
			else
			{
				CrosshairinAirFactor
					= FMath::FInterpTo(CrosshairinAirFactor, 0.f, DeltaTime, 30.f);
			}// 기본 값, 범위 값, 적용 시간, 적용 시간 수치(얼마나 빠르게)
			// 앉기도 구현하는가?
			
			
			if (bIsAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor,0.58f , DeltaTime,30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}
			// 총을 쐈을 때 벌어지는 수치가 점점 0으로 줄어드는 방식
			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime,CrosshairShootingFactor);

			HUDPackage.CrosshairsSpread =
				CrosshairBaseFactor	+ CrosshairVelocityFactor 
				+ CrosshairinAirFactor
				- CrosshairAimFactor
				+ CrosshairShootingFactor;
			CurrentWeaponSpread = HUDPackage.CrosshairsSpread;
			PlayerCrosshairHUD->SetHUDPackage(HUDPackage);
		}
	
	}


}

void UPlayerCombatComponent::UpdateHealth()
{
	PlayerCrosshairHUD->CombatOverlay->HUDCurrentHealth = PlayerCurrentHealth;
	PlayerCrosshairHUD->CombatOverlay->HUDMaxHealth = PlayerMaxHealth;

	PlayerCrosshairHUD->CombatOverlay->UpdateHPSeg(PlayerCurrentHealth, PlayerMaxHealth);

}

void UPlayerCombatComponent::SetHUDHealth(float CurrentHealth, float MaxHealth)
{

	if (PlayerController)
	{
		PlayerCrosshairHUD = PlayerCrosshairHUD == nullptr ?
			Cast<AWeaponCrosshairHUD>(PlayerController->GetHUD()) : PlayerCrosshairHUD;
		bool bHUDValid = PlayerCrosshairHUD &&
			PlayerCrosshairHUD->CombatOverlay;

		if (bHUDValid)
		{
			PlayerCrosshairHUD->CombatOverlay->HUDCurrentHealth = PlayerCurrentHealth;
			PlayerCrosshairHUD->CombatOverlay->HUDMaxHealth = PlayerMaxHealth;
			PlayerCrosshairHUD->InitHUDMaxHealth = PlayerMaxHealth;
			if (!bInitHpSeg)
			{
				PlayerCrosshairHUD->CombatOverlay->CreateHPSeg(PlayerMaxHealth);
				PlayerCrosshairHUD->CombatOverlay->UpdateHPSeg(PlayerCurrentHealth,PlayerMaxHealth);
				bInitHpSeg = true;
			}

		}
		
	}
}

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController = Cast<AMyPlayerController>(PC);
	
	InitializeWeaponParts(); // 무기 정보들 넣기

	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetFollowCamera())
		{
			DefaultFOV = PlayerCharacter->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
		
	}

	PlayerInventory = PlayerCharacter->Inventory;
}

void UPlayerCombatComponent::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (PlayerCharacter == nullptr || WeaponToEquip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 또는 장비 nullptr"));
		return;
	}
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("재장전 중에는 무기를 새로 장비할 수 없습니다."));
		return;
	}

	// 소켓에 장비 장착
	if (EquippedWeapon)
	{
		EquippedWeapon->Dropped();
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWT_Equipped);

	
	const USkeletalMeshSocket* HandSocket =
		PlayerCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, PlayerCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(PlayerCharacter);
	EquippedWeapon->OnWeaponEquipped(PlayerCharacter,PlayerController);
	// 새로운 무기에 대한 HUD 출력

	if (PlayerController->GetWeaponCrosshairHUD())
	{
		PlayerController->InitHUDWeaponAmmo(EquippedWeapon->GetCurrrentWeaponAmmo(),
			EquippedWeapon->GetMaxWeaponAmmo());
		PlayerController->SetHUDWeaponAmmo(EquippedWeapon->GetCurrrentWeaponAmmo());
	}

	if (PlayerInventory)
	{
		if (PlayerController->GetWeaponCrosshairHUD())
		{
			int32 InvenAmmo =
				PlayerInventory->ReturnCurrentAmmo(EquippedWeapon->GetWeaponType());
			if (InvenAmmo != -1)
			{
				PlayerController->SetHUDCarriedAmmo(InvenAmmo);
			}
			else
			{
				PlayerController->SetHUDCarriedAmmo(0);
				UE_LOG(LogTemp, Warning, TEXT("인벤토리에 탄 없음"));
			}
			
		}
	}
}

void UPlayerCombatComponent::FireButtonPressed(bool bPressed)
{
	// 발사
	if (!EquippedWeapon) return;
	bFireButtonPressed = bPressed;
	// 애니메이션 재생
	if (PlayerCharacter&& bFireButtonPressed)
	{
		ComponentFire();
	}



}

void UPlayerCombatComponent::ComponentFire()
{
	if (WeaponCanFire())
	{
		bIsCanFireinRate = false;
		PlayerCharacter->PlayFireMontage(bIsAiming);
		EquippedWeapon->Fire(HitTargetPos,CurrentWeaponSpread);

		// 총을 쐈을 떄 벌어질 조준선에 크기에 더해질 값
		CrosshairShootingFactor += 0.75f;

		StartFireTimer();
	}

}

void UPlayerCombatComponent::UpdateReloadUI()
{
	if (PlayerCharacter == nullptr|| PlayerController==nullptr || EquippedWeapon == nullptr) return;

	float RemainingReloadTime = PlayerCharacter->GetWorldTimerManager().GetTimerRemaining
	(FReloadTimerHandle);

	PlayerController->GetWeaponCrosshairHUD()->UpdateReloadUIProgress(
		RemainingReloadTime, EquippedWeapon->GetTimeToFinishReload());

}

void UPlayerCombatComponent::ApplyReloadUI()
{
	PlayerController->GetWeaponCrosshairHUD()->SetReloadUIVisibility(true);
}

void UPlayerCombatComponent::DeApplyReloadUI()
{
	PlayerController->GetWeaponCrosshairHUD()->SetReloadUIVisibility(false);
}

void UPlayerCombatComponent::StartWeaponReload()
{
	if (EquippedWeapon && !bIsReloading)
	{
		if (PlayerInventory->ReturnCurrentAmmo(EquippedWeapon->GetWeaponType())<=0)
		{// 탄없음
			UE_LOG(LogTemp, Warning, TEXT("탄없음 재장전 불가"));
			return;
		}
		else
		{
			if (EquippedWeapon->GetCurrrentWeaponAmmo() == EquippedWeapon->GetMaxWeaponAmmo())
			{
				UE_LOG(LogTemp, Warning, TEXT("탄 꽉찼음., 재장전 불가"));
				return;
			}
			else
			{
				if (PlayerCharacter)
				{
					bIsReloading = true;
					ApplyReloadUI();
					PlayerCharacter->GetWorldTimerManager().SetTimer(
						FReloadTimerHandle,
						this,
						&UPlayerCombatComponent::OnFinishWeaponReload,
						EquippedWeapon->GetTimeToFinishReload());
				}
			}
			
		}

	}
	
}


void UPlayerCombatComponent::OnFinishWeaponReload()
{
	if (EquippedWeapon)
	{

		EquippedWeapon->Reload();
		DeApplyReloadUI();
		bIsReloading = false;
	}
}

void UPlayerCombatComponent::OnInfiniteAmmoStart(float InfiniteTimerAmount)
{
	if (PlayerCharacter)
	{
		bIsInfiniteAmmo = true;
		PlayerCharacter->GetWorldTimerManager().SetTimer(
			FInfiniteAmmo,
			this,
			&UPlayerCombatComponent::OnInfiniteAmmoEnd,
			InfiniteTimerAmount,
			false);
	}
}

void UPlayerCombatComponent::OnInfiniteAmmoEnd()
{
	if (PlayerCharacter)
	{
		bIsInfiniteAmmo = false;
		PlayerCharacter->GetWorldTimerManager().ClearTimer(FInfiniteAmmo);
	}
}

bool UPlayerCombatComponent::IsPlayerDead()
{
	return PlayerCurrentHealth <= 0;
}

EWeaponPartsType UPlayerCombatComponent::GetWeaponPartsTypefromFName(FName ItemKey)
{
	if (!WeaponPartsDataTable) return EWeaponPartsType::EWT_None;

	for (FWeaponPartsTableRow* Row : WeaponpartsTableRows)
	{
		if (Row && Row->ItemKey == ItemKey)
		{
			return Row->WeaponPartsType;
		}
	}

	return EWeaponPartsType::EWT_None; // 해당 ItemKey를 찾지 못한 경우
}

FName UPlayerCombatComponent::GetItemKeyFromWeaponPartsType(EWeaponPartsType WTP)
{
	if (!WeaponPartsDataTable) return NAME_None;

	for (FWeaponPartsTableRow* Row : WeaponpartsTableRows)
	{
		if (Row && Row->WeaponPartsType == WTP)
		{
			return Row->ItemKey;
		}
	}

	return NAME_None; // 해당 WeaponPartsType을 찾지 못한 경우
}

void UPlayerCombatComponent::UnlockWeaponParts(FName ItemKey)
{
	if (AvailableWeaponParts.Contains(ItemKey) && GetWeaponPartsTypefromFName(ItemKey) != EWeaponPartsType::EWT_None)
	{
		AvailableWeaponParts[ItemKey] = true; // 부착물 사용 가능하도록 설정
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("존재하지 않는 무기 파츠 ItemKey"));
	}
	
}

void UPlayerCombatComponent::InitializeWeaponParts()
{
	if (!WeaponPartsDataTable) return;

	WeaponPartsDataTable->GetAllRows<FWeaponPartsTableRow>(TEXT("GetAllRowsFromWeaponPartsTable"), WeaponpartsTableRows);

	for (FWeaponPartsTableRow* Row : WeaponpartsTableRows)
	{
		if (Row && GetWeaponPartsTypefromFName(Row->ItemKey) != EWeaponPartsType::EWT_None)
		{	// 넘겨받은 ItemKey가 실제로 부품 테이블에 있는지 검증
			//  TMap에 저장 (초기 상태는 false = 아직 사용 불가능)
			AvailableWeaponParts.Add(Row->ItemKey, false);
		}
	}
}

bool UPlayerCombatComponent::IsWeaponPartsAvailable(FName ItemKey)
{
	return AvailableWeaponParts[ItemKey];
}



void UPlayerCombatComponent::SetAiming(bool _bIsAiming)
{
	if (PlayerCharacter == nullptr || EquippedWeapon == nullptr)
	{
		return;
	}

	bIsAiming = _bIsAiming;

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Sniper)
	{
		PlayerCharacter->ShowSniperScopeWidget(bIsAiming);
	}


}

void UPlayerCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr)
	{
		return;
	}

	if (bIsAiming)
	{
		CurrentFOV=FMath::FInterpTo(
			CurrentFOV,
			EquippedWeapon->GetWeaponZoomFov(),
			DeltaTime,EquippedWeapon->GetWeaponZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(
			CurrentFOV,
			DefaultFOV,
			DeltaTime, ZoomInterpSpeed);
	}
	if (PlayerCharacter && PlayerCharacter->GetFollowCamera())
	{
		PlayerCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

AWeaponBase* UPlayerCombatComponent::GetEquippedWeapon()
{
	return EquippedWeapon != nullptr ? EquippedWeapon:nullptr;
}

float UPlayerCombatComponent::GetCurrentPlayerHealth()
{
	return PlayerCurrentHealth;
}

void UPlayerCombatComponent::SetCurrentPlayerHealth(float _HP)
{
	PlayerCurrentHealth = _HP;
}

float UPlayerCombatComponent::GetMaxPlayerHealth()
{
	return PlayerMaxHealth;
}

void UPlayerCombatComponent::SetMaxPlayerHealth(float _HP)
{
	PlayerMaxHealth = _HP;
}

bool UPlayerCombatComponent::WeaponCanFire()
{
	if (EquippedWeapon == nullptr) return false;

	return !EquippedWeapon->IsWeaponMagEmpty() && bIsCanFireinRate && !bIsReloading
		&& !PlayerCharacter->bIsInventoryVisible;		
}

void UPlayerCombatComponent::StartFireTimer()
{
	if (EquippedWeapon == nullptr || PlayerCharacter == nullptr)
	{
		return;
	}
	PlayerCharacter->GetWorldTimerManager().SetTimer(
	FireTimer,
		this,
		&UPlayerCombatComponent::FireTimerFinished,
		EquippedWeapon->WeaponFireRate);
}

void UPlayerCombatComponent::FireTimerFinished()
{
	bIsCanFireinRate = true;
	if (bFireButtonPressed&&EquippedWeapon&&EquippedWeapon->bIsWeaponAutomatic)
	{
		ComponentFire();
	}

}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
	
	SetHUDCrosshairs(DeltaTime);
	SetHUDHealth(PlayerCurrentHealth, PlayerMaxHealth);
	UpdateReloadUI();
	InterpFOV(DeltaTime);
}

