#include "MyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h" //뷰포트에 로그를 출력 위함
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
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SprintSpeed = WalkSpeed * SprintSpeedMultiplier;
	SlowWalkSpeed = WalkSpeed * SlowWalkSpeedMultiplier;
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
			if (PlayerController->ProneAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ProneAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartProne
				);
			}

			if (PlayerController->ProneAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ProneAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopProne
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
		}
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	OnPickupItem();
	HideCameraIfCharacterClose();
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
		int32 Quantity = 1; 

		if (Inventory)
		{
			bool bAdded = Inventory->AddItem(ItemKey, Quantity);
			if (bAdded)
			{
				UE_LOG(LogTemp, Log, TEXT("Character to Inventory %s added to inventory."), *ItemKey.ToString());
				OverlappingItem->Destroy();
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
		UE_LOG(LogTemp, Warning, TEXT("No overlapping item to pick up."));
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
void AMyCharacter::ToggleInventory(const FInputActionValue& Value)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !Inventory || !Inventory->InventoryWidget)
	{
		return;
	}

	if (Inventory->InventoryWidget->IsVisible())
	{
		Inventory->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	
	}
	else
	{
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

	const FVector2D MoveInput = value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRatotion = FRotator(0, Rotation.Yaw, 0);
	const FVector ForwardDir = FRotationMatrix(YawRatotion).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRatotion).GetUnitAxis(EAxis::Y);

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(ForwardDir, MoveInput.X * MouseSensitivity);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(RightDir, MoveInput.Y * MouseSensitivity);
	}


}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
	// 개발용, 무기 주으면 바로 장착할 수 있도록 구현
	if (value.Get<bool>())
	{
		// 지금은 바로 낄수있도록 구현합니다.
		if (CombatComponent)
		{
			AWeaponBase* WeaponToEquip =
				Cast<AWeaponBase>(PickableItem);


			if (WeaponToEquip && CombatComponent->EquippedWeapon == nullptr)
			{
				// 주울수있는 아이템이 무기 인경우 && 빈손인 경우
				CombatComponent->EquipWeapon(WeaponToEquip);

				//임시 추가:전지현
				bHasWeapon = true;
			}
			else if (ABaseItem* ItemToPickUp =
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
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopPickUp!"));
			}
		}
	}
}

void AMyCharacter::StartProne(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartProne!"));
			}
		}
	}
}

void AMyCharacter::StopProne(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopProne!"));
			}
		}
	}
}

void AMyCharacter::StartCrouch(const FInputActionValue& value)
{
	if (!bIsCrouching)
	{
		bIsCrouching = true;

		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartCrouch!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = SlowWalkSpeed;
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
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopCrouch!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartSlowWalking!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = SlowWalkSpeed;
	}
}

void AMyCharacter::StopSlowWalking(const FInputActionValue& value)
{
	if (bIsSlowWalking)
	{
		bIsSlowWalking = false;

		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopSlowWalking!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyCharacter::StartReload(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartReload!"));
			}
		}
	}
}

void AMyCharacter::StopReload(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopReload!"));
			}
		}
	}
}

void AMyCharacter::StartFire(const FInputActionValue& value)
{
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(true);
	}
}

void AMyCharacter::StopFire(const FInputActionValue& value)
{
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
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


void AMyCharacter::SetPickableItem(ABaseItem* OverlappedItem)
{
	PickableItem = OverlappedItem;
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
	if (Inventory && Inventory->InventorySubsystem)
	{
		return Inventory->InventorySubsystem->GetGold();
	}
	return 0;
}
const TArray<FInventoryConsumable>& AMyCharacter::GetConsumableItems() const
{
	if (Inventory && Inventory->InventorySubsystem)
	{
		return Inventory->InventorySubsystem->GetConsumableItems();
	}
	static TArray<FInventoryConsumable> EmptyConsumables;
	return EmptyConsumables;
}
const TArray<FInventoryEquipment>& AMyCharacter::GetEquipmentItems() const
{
	if (Inventory && Inventory->InventorySubsystem)
	{
		return Inventory->InventorySubsystem->GetEquipmentItems();
	}
	static TArray<FInventoryEquipment> EmptyEquipments;
	return EmptyEquipments;
}
const TArray<FInventoryOthers>& AMyCharacter::GetOthersItems() const
{
	if (Inventory && Inventory->InventorySubsystem)
	{
		return Inventory->InventorySubsystem->GetOthersItems();
	}
	static TArray<FInventoryOthers> EmptyOthers;
	return EmptyOthers;
}
bool AMyCharacter::AddItem(const FName& ItemKey, int32 Quantity)
{
	if (Inventory)
	{
		return Inventory->AddItem(ItemKey, Quantity);
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

