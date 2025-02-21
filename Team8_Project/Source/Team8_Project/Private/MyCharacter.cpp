#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "InventoryComponent.h"
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
//
//// Called every frame
//void AMyCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
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
	// 겹침 종료된 액터가 OverlappingItem이라면 초기화합니다.
	if (OtherActor && OtherActor == OverlappingItem)
	{
		UE_LOG(LogTemp, Log, TEXT("Item ended overlap: %s"), *OtherActor->GetName());
		OverlappingItem = nullptr;
	}
}
