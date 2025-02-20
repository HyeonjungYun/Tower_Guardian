// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"
#include "WeaponBase.h"
#include "../MyCharacter.h"
#include "Engine/SkeletalMeshSocket.h"// 소켓
#include "Kismet/GameplayStatics.h"//crosshair
#include "DrawDebugHelpers.h"
UPlayerCombatComponent::UPlayerCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true; //디버그 틱
	PlayerCharacter = nullptr;
	EquippedWeapon = nullptr;
}

void UPlayerCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize = FVector2D::ZeroVector;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}// 매개변수에게 뷰포트 값이붙음(OUT 매크로)

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
		FVector Start = CrosshairWorldDirection;

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH_FOR_CROSSHAIR; // 시작점 + 방향*거리

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End; // 거리안에 닿는것이 없다면 마지막을
			HitTargetPos = End;
		}
		else
		{
			HitTargetPos = TraceHitResult.ImpactPoint;
			// 디버그출력
			DrawDebugSphere(GetWorld(), 
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red);
		}

	}
}


void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPlayerCombatComponent::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (PlayerCharacter == nullptr || WeaponToEquip == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 또는 장비 nullptr"));
		return;
	}
	// 소켓에 장비 장착
	
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWT_Equipped);

	
	const USkeletalMeshSocket* HandSocket =
		PlayerCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, PlayerCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(PlayerCharacter);
	
}

void UPlayerCombatComponent::FireButtonPressed(bool bPressed)
{
	// 발사
	if (!EquippedWeapon) return;
	bFireButtonPressed = bPressed;
	// 애니메이션 재생
	if (PlayerCharacter&& bFireButtonPressed)
	{
		PlayerCharacter->PlayFireMontage(bIsAiming);
		EquippedWeapon->Fire(HitTargetPos);
	
	}



}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}

