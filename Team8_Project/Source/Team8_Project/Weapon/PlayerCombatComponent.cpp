// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"
#include "WeaponBase.h"
#include "../MyCharacter.h"
#include "Engine/SkeletalMeshSocket.h"// 소켓
UPlayerCombatComponent::UPlayerCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	PlayerCharacter = nullptr;
	EquippedWeapon = nullptr;
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
	bFireButtonPressed = bPressed;
	// 애니메이션 재생
	if (PlayerCharacter&& bFireButtonPressed)
	{
		PlayerCharacter->PlayFireMontage(bIsAiming);
		EquippedWeapon->Fire();
	
	}



}



