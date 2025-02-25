#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "InventoryComponent.h"
#include "InventorySubsystem.h"
#include "InventoryWidget.h"
#include "Components/CapsuleComponent.h"
#include "BaseItem.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"


AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::World);

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	//add
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);
	///
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
			if (PlayerController->PickUpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PickUpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::PickUp
				);
			}
			if (PlayerController->InventoryToggleAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InventoryToggleAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::ToggleInventory
				);
			}
		}
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}

	FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}
void AMyCharacter::PickUp (const FInputActionValue& Value)
{
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
//
// ------------------------------
// Interface Implementation
// ------------------------------
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

