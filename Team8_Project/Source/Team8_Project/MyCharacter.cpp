#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/PlayerCombatComponent.h"
#include "Weapon/WeaponBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "BaseItem.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventorySubsystem.h"
#include "Team8_Project/Weapon/WeaponType.h"
#include "Team8_Project/WorldSpawnUISubSystem.h"
#include "Team8_Project/GameState/CH8_GameState.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = GetCapsuleComponent(); //기본 캡슐 컴포넌트만 반환
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);
  

	//Capsule컴포넌트 -> Overlap에 이벤트 바인딩
	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComponent"));

	// mesh 카메라 관련 충돌
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// PlayerController 받아오기
	MyPlayerController = Cast<AMyPlayerController>(Controller);
	if (MyPlayerController)
	{
		// 초기화를 위한 현재 정보를 HUD로 넘기려면 여기서
		// PC->HUD->HUD의 각종 UI 오버레이등을 사용
	}
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SprintSpeed = WalkSpeed * SprintSpeedMultiplier;
	SlowWalkSpeed = WalkSpeed * SlowWalkSpeedMultiplier;
	FunchAnimMaxIndex = PunchMontages.Num();
	PreWeaponType = EWeaponType::EWT_None;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//액션 바인딩
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartJump
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopJump
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSprint
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSprint
				);
			}

			//
			if (PlayerController->PickUpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PickUpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartPickUp
				);
			}

			if (PlayerController->PickUpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PickUpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopPickUp
				);
			}

			//
			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartCrouch
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopCrouch
				);
			}
			//
			if (PlayerController->SlowWalkingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SlowWalkingAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSlowWalking
				);
			}

			if (PlayerController->SlowWalkingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SlowWalkingAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSlowWalking
				);
			}
			//
			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartReload
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopReload
				);
			}
			//
			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::StartFire
				);
			}

			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopFire
				);
			}
			if (PlayerController->AimingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::OnAiming
				);
			}
			if (PlayerController->AimingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::ReleaseAiming
				);
			}
			if (PlayerController->InventoryToggleAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InventoryToggleAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::ToggleInventory
				);
			}
			if (PlayerController->WeaponModifyingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->WeaponModifyingAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::OnWeaponModifyingInput
				);
			}
		}
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	HideCameraIfCharacterClose();
	CalculateRotation(DeltaTime);
	OnPickupItem();
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overl"));
	if (OtherActor && OtherActor != this)
	{
		ABaseItem* Item = Cast<ABaseItem>(OtherActor);
		if (Item)
		{
			OverlappingItem = Item;
			UE_LOG(LogTemp, Log, TEXT("Item overlapped: %s"), *Item->GetName());
		}
	}
}

void AMyCharacter::OnPickupItem()
{

	if (OverlappingItem)
	{
		FName ItemKey = OverlappingItem->GetItemType();
		EItemType SlotType = OverlappingItem->GetSlotType();
		int32 Quantity = OverlappingItem->GetQuantity();

		if (Inventory)
		{
			bool bAdded = AddItem(ItemKey, Quantity, SlotType);
			if (bAdded)
			{
				UE_LOG(LogTemp, Warning, TEXT("Character to Inventory %s added to inventory."), *ItemKey.ToString());
				OverlappingItem->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("AfterDestory"));
				OverlappingItem = nullptr;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to add item %s."), *ItemKey.ToString());
			}
		}

	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No overlapping item to pick up."));
	}


}
void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == OverlappingItem)
	{
		UE_LOG(LogTemp, Log, TEXT("Item ended overlap: %s"), *OtherActor->GetName());
		OverlappingItem = nullptr;
	}
}
void AMyCharacter::ToggleInventory(const FInputActionValue& Value = FInputActionValue())
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !Inventory || !Inventory->InventoryWidget)
	{
		return;
	}

	if (Inventory->InventoryWidget->IsVisible())
	{
		bIsInventoryVisible = false;
		//상태 추가 
		Inventory->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		
	
	}
	else
	{
		bIsInventoryVisible = true;
		Inventory->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		Inventory->InventoryWidget->UpdateInventoryUI();
		//입력이 ui에 전달되고 그다음 게임쪽으로 전달된다
		FInputModeGameAndUI  InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		/*InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);*/
		InputMode.SetHideCursorDuringCapture(false);  // Added this line
		/*InputMode.SetWidgetToFocus(Inventory->InventoryWidget->TakeWidget());*/
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
		//뷰포트에 붙였을 때 
	}

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->PlayerCharacter = this;
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller)
		return;

	//카메라 바라보는 방향 = 캐릭터 바라보는 방향
	const FVector2D MoveInput = value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	SetActorRotation(NewRotation);

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(ForwardDir, MoveInput.X * MouseSensitivity);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(RightDir, MoveInput.Y * MouseSensitivity);
	}

	//함수 안에서 조건 검사
	StopPunch();
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddControllerYawInput(LookInput.X * MouseSensitivity);
	}

	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		AddControllerPitchInput(LookInput.Y * MouseSensitivity);
	}


}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	//장전중 달리기 막음
	if (PlayerStates == EPlayerStateType::EWT_Reload)
	{
		return;
	}

	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void AMyCharacter::StartPickUp(const FInputActionValue& value)
{
	//장전중 줍기 막음
	if (PlayerStates == EPlayerStateType::EWT_Reload)
	{
		return;
	}

	// 개발용, 무기 주으면 바로 장착할 수 있도록 구현
	if (value.Get<bool>())
	{
		// 지금은 바로 낄수있도록 구현합니다.
		if (CombatComponent)
		{
			AWeaponBase* WeaponToEquip =
				Cast<AWeaponBase>(PickableWeapon);

			if (WeaponToEquip)
			{
				// 주울수있는 아이템이 무기 인경우 && 빈손인 경우
				CombatComponent->EquipWeapon(WeaponToEquip);

				//교체 애니메이션을 위한 추가
				if (PreWeaponType != WeaponToEquip->GetWeaponType())
				{
					PlayerStates = EPlayerStateType::EWT_ChangeWeapon;
				}
			}
			if (ABaseItem* ItemToPickUp =
				Cast<ABaseItem>(PickableItem))
			{
				OnPickupItem();// 인벤토리용
			}
		}
	}
}

void AMyCharacter::StopPickUp(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
	}
}

void AMyCharacter::StartCrouch(const FInputActionValue& value)
{
	if (!bIsCrouching)
	{
		bIsCrouching = true;

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SlowWalkSpeed;
		}
		Crouch(bIsCrouching);
	}

}

void AMyCharacter::StopCrouch(const FInputActionValue& value)
{
	if (bIsCrouching)
	{
		bIsCrouching = false;

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
		UnCrouch(bIsCrouching);
	}
}

void AMyCharacter::StartSlowWalking(const FInputActionValue& value)
{
	if (!bIsSlowWalking)
	{
		bIsSlowWalking = true;

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SlowWalkSpeed;
		}
	}
}

void AMyCharacter::StopSlowWalking(const FInputActionValue& value)
{
	if (bIsSlowWalking)
	{
		bIsSlowWalking = false;

		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void AMyCharacter::StartReload(const FInputActionValue& value)
{
	//장착된 총알이 꽉 차있으면 return, 하나라도 소비한 상태면 실행
	if (CombatComponent->GetEquippedWeapon())
	{
		int32 cur = CombatComponent->GetEquippedWeapon()->GetCurrrentWeaponAmmo();
		int32 max = CombatComponent->GetEquippedWeapon()->GetMaxWeaponAmmo();
		if (cur == max)
		{
			PlayerStates = EPlayerStateType::EWT_Normal;
			return;
		}
		if (CombatComponent->GetEquippedWeapon()->GetWeaponType() == EWeaponType::EWT_Shotgun)
		{
			ShotgunReloadCount = max - cur;
		}
	}

	PlayerStates = EPlayerStateType::EWT_Reload;

	if (value.Get<bool>())
	{
		CombatComponent->StartWeaponReload();
	}
}

void AMyCharacter::StopReload(const FInputActionValue& value)
{
}

int32 AMyCharacter::GetShotgunReloadCount()
{
	return ShotgunReloadCount;
}

void AMyCharacter::StartFire(const FInputActionValue& value)
{
	//장전중 발사 막음
	if (PlayerStates == EPlayerStateType::EWT_Reload)
	{
		return;
	}

	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(true);
		PlayerStates = EPlayerStateType::EWT_Fire;

		if (CombatComponent->GetEquippedWeapon())
		{
			if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
			{
				if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
					CameraManager->StartCameraShake(CameraShakeClass, 1.f);
			}
		}
	}

	//펀치 발동 조건
	if (!CombatComponent->GetEquippedWeapon())
	{
		PlayerStates = EPlayerStateType::EWT_Punch;
		FunchCombo(FunchAnimIndex);
	}
}

void AMyCharacter::StopFire(const FInputActionValue& value)
{
	PlayerStates = EPlayerStateType::EWT_Normal;

	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
}

void AMyCharacter::CalculateRotation(float DeltaTime)
{
	const FRotator Rotation = Controller->GetControlRotation();
	FRotator ActorRot = GetActorRotation();
	FRotator InterpRotation = FMath::RInterpTo(ActorRot, Rotation, DeltaTime, RotationSpeed);
	InterpRotation.Pitch = 0.f;
	InterpRotation.Roll = 0.f;
	NewRotation = InterpRotation;
}

void AMyCharacter::OnAiming()
{
	if (CombatComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("전투 컴포넌트 없음"));
		return;
	}
	else
	{
		CombatComponent->SetAiming(true);

	}

}

void AMyCharacter::ReleaseAiming()
{
	if (CombatComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("전투 컴포넌트 없음"));
		return;
	}
	else
	{
		CombatComponent->SetAiming(false);

	}
}

void AMyCharacter::OnWeaponModifyingInput()
{
	UE_LOG(LogTemp, Warning, TEXT("Button WeaponModifying started"));
}

void AMyCharacter::HideCameraIfCharacterClose()
{
	if ((Camera->GetComponentLocation()-GetActorLocation()).Size()<CameraThreshold)
	{
		GetMesh()->SetVisibility(false);
		if (CombatComponent && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponMesh())
		{
			// 소유자=>유저1번 만 안보이게 
			CombatComponent->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (CombatComponent && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponMesh())
		{
			// 소유자=>유저1번 만 안보이게 
			CombatComponent->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}

void AMyCharacter::PlayFireMontage(bool bAiming)
{
	if (CombatComponent == nullptr ||
		CombatComponent->EquippedWeapon == nullptr)
	{
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireRifleAnimMontage)
	{
		AnimInstance->Montage_Play(FireRifleAnimMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

UPlayerCombatComponent* AMyCharacter::GetCombatComponent()
{
	return CombatComponent;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float CurrentHealth =  FMath::Clamp(
		CombatComponent->GetCurrentPlayerHealth() - DamageAmount
		,0
		,CombatComponent->GetMaxPlayerHealth());
	
	SetHP(CurrentHealth);

	PlayerStates = EPlayerStateType::EWT_Damaged;

	FVector DamageTextLoc = GetActorLocation();
	DamageTextLoc.Z += GetComponentByClass<UShapeComponent>()->Bounds.BoxExtent.Z * 1.2f;
	float RandomValue = FMath::RandRange(-2.f, 2.f);
	DamageTextLoc.X += GetComponentByClass<UShapeComponent>()->Bounds.BoxExtent.X * RandomValue;
	GetGameInstance()->GetSubsystem<UWorldSpawnUISubSystem>()->SpawnDamageText(GetWorld(), DamageAmount, DamageTextLoc);

	// HUD 갱신
	CombatComponent->UpdateHealth();

	// 사망 확인
	if (CombatComponent->IsPlayerDead())
	{
		// 사망 처리 애니메이션 재생, UI 출력, GameState 호출 등등 처리 해주세요
		OnDeath();
	}
	
	return DamageAmount;
}

void AMyCharacter::OnDeath()
{
	PlayerStates = EPlayerStateType::EWT_Dead;
	UE_LOG(LogTemp, Warning, TEXT("플레이어 사망 확인"));

	if (ACH8_GameState* GameState = Cast<ACH8_GameState>(GetWorld()->GetGameState()))
	{
		GameState->EndGame(false);
	}
}


void AMyCharacter::SetPickableItem(ABaseItem* OverlappedItem)
{
	PickableItem = OverlappedItem;
}

void AMyCharacter::SetPickableWeapon(AWeaponBase* OverlappedWeapon)
{
	if (OverlappedWeapon)
	{
		PickableWeapon = OverlappedWeapon;
	}

}

void AMyCharacter::SortEquipmentItems(bool bIsAscending)
{
	if (Inventory)
	{
		Inventory->SortEquipmentItems(bIsAscending);
	}
}
void AMyCharacter::SortConsumableItems(bool bIsAscending)
{
	if (Inventory)
	{
		Inventory->SortConsumableItems(bIsAscending);
	}
}
void AMyCharacter::SortOthersItems(bool bIsAscending)
{
	if (Inventory)
	{
		Inventory->SortOthersItems(bIsAscending);
	}
}
int32 AMyCharacter::GetGold() const
{
	if (Inventory)
	{
		return Inventory->GetGold();
	}
	return 0;
}
const TArray<FInventoryConsumable>& AMyCharacter::GetConsumableItems() const
{
	if (Inventory)
	{
		return Inventory->GetConsumableItems();
	}
	static TArray<FInventoryConsumable> EmptyConsumables;
	return EmptyConsumables;
}
const TArray<FInventoryEquipment>& AMyCharacter::GetEquipmentItems() const
{
	if (Inventory)
	{
		return Inventory->GetEquipmentItems();
	}
	static TArray<FInventoryEquipment> EmptyEquipments;
	return EmptyEquipments;
}
const TArray<FInventoryOthers>& AMyCharacter::GetOthersItems() const
{
	if (Inventory)
	{
		return Inventory->GetOthersItems();
	}
	static TArray<FInventoryOthers> EmptyOthers;
	return EmptyOthers;
}

float AMyCharacter::GetHP() const
{
	if (CombatComponent)
	{
		return CombatComponent->GetCurrentPlayerHealth();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("컴뱃 컴포넌트 없음"));
		return -1;
	}

}
void AMyCharacter::SetHP(float Value)
{
	CombatComponent->SetCurrentPlayerHealth(Value);
}

bool AMyCharacter::AddItem(const FName& ItemKey, int32 Quantity,EItemType ItemType)
{
	if (Inventory)
	{
		return Inventory->AddItem(ItemKey, Quantity, ItemType);
	}
	return false;
}
bool AMyCharacter::RemoveItem(const FName& ItemKey, int32 Quantity)
{
	if (Inventory)
	{
		return Inventory->RemoveItem(ItemKey, Quantity);
	}
	return false;
}
bool AMyCharacter::UseItem(int32 SlotIndex, EItemType ItemType)
{
	if (Inventory)
	{
		return Inventory->UseItem(SlotIndex, ItemType);
	}
	return false;
}
void AMyCharacter::SetGold(int32 NewGold)
{
	if (Inventory)
	{
		Inventory->SetGold(NewGold);
	}
}
void AMyCharacter::SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType)
{
	check(Inventory);
	Inventory->SwapItem(PrevIndex, CurrentIndex, PrevSlotType, CurrentSlotType);
}
const TArray<FInventoryAmmo>& AMyCharacter::GetAmmoItems() const
{
	if (Inventory)
	{
		return Inventory->GetAmmoItems();
	}
	static TArray<FInventoryAmmo> EmptyAmmos;
	return EmptyAmmos;
}
int32 AMyCharacter::SearchItemByNameAndType(const FName& ItemKey, const EItemType& ItemType) const
{
	if (Inventory) 
	{
		int32 Result = Inventory->SearchItemByNameAndType(ItemKey, ItemType);
		return Result;
	}
	return INDEX_NONE;
}
int32 AMyCharacter::SearchItemByName(const FName& ItemKey) const
{
	if (Inventory)
	{
		int32 Result = Inventory->SearchItemByName(ItemKey);
		return Result;
	}
	return INDEX_NONE;
}
void AMyCharacter::SortAmmoItems(bool bIsAscending)
{
	if (Inventory)
	{
		Inventory->SortAmmoItems(bIsAscending);
	}
}

void AMyCharacter::FunchCombo(int32 AnimIndex)
{
	CurrentFunchAnimIndex = FunchAnimIndex;

	//타이머 발동
	GetWorldTimerManager().ClearTimer(FunchComboTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(FunchComboTimerHandle, this, &AMyCharacter::ResetFunchCombo, 1.0f, false);

	//플레이 애님몽타주
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PunchMontages.IsValidIndex(FunchAnimIndex))
	{
		AnimInstance->Montage_Play(PunchMontages[FunchAnimIndex]);
	}
	
	//1초 안에 누르면 카운트 증가
	FunchAnimIndex++;
	if (FunchAnimIndex >= FunchAnimMaxIndex)
	{
		FunchAnimIndex = 0;
	}
}

void AMyCharacter::ResetFunchCombo()
{
	//1초 후 콤보 카운트 리셋
	FunchAnimIndex = 0;
}

void AMyCharacter::StopPunch()
{
	if (PlayerStates == EPlayerStateType::EWT_Punch)
	{
		PlayerStates = EPlayerStateType::EWT_Normal;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && PunchMontages.IsValidIndex(CurrentFunchAnimIndex))
		{
			AnimInstance->Montage_Stop(0.0f, PunchMontages[CurrentFunchAnimIndex]);
		}
	}
}

void AMyCharacter::ApplySpeedBoost(float BoostPercent, float Duration)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		if (GetWorldTimerManager().IsTimerActive(SpeedBoostTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(SpeedBoostTimerHandle);
			MoveComp->MaxWalkSpeed = BaseWalkSpeed;
			SprintSpeed = BaseWalkSpeed * SprintSpeedMultiplier;
		}
		//Tick등에서 MoveComp->MaxWalkSpeed를 항상 업데이트를 해주는 방식이 아니기때문에 
		//직접 값을 변경해줘야 한다.
		float NewWalkSpeed = BaseWalkSpeed * (1.0f + BoostPercent / 100.0f);
		float NewSprintSpeed = BaseWalkSpeed * SprintSpeedMultiplier * (1.0f + BoostPercent / 100.0f);

		MoveComp->MaxWalkSpeed = NewWalkSpeed;
		WalkSpeed = NewWalkSpeed;
		SprintSpeed = NewSprintSpeed;

		// 타이머 설정: 지속시간 후에 원래 속도로 복원
		GetWorldTimerManager().SetTimer(SpeedBoostTimerHandle, FTimerDelegate::CreateLambda([this]()
			{
				if (UCharacterMovementComponent* MoveCompInner = GetCharacterMovement())
				{
					MoveCompInner->MaxWalkSpeed = BaseWalkSpeed;
					WalkSpeed = BaseWalkSpeed;
					SprintSpeed = BaseWalkSpeed * SprintSpeedMultiplier;
				}
			}), Duration, false);
	}
}
//블루 프린트 용
float AMyCharacter::GetPlayerSpeed()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	return MoveComp->MaxWalkSpeed;
}

float AMyCharacter::GetMaxHP()
{
	return CombatComponent->PlayerMaxHealth;
}