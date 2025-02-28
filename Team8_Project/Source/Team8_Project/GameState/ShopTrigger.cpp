#include "ShopTrigger.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/GameState/ShopComponent.h"
#include "Team8_Project/GameState/CH8_GameState.h"
#include "Team8_Project/MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

AShopTrigger::AShopTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(100.0f));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AShopTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AShopTrigger::OnOverlapEnd);

	ShopComponent = CreateDefaultSubobject<UShopComponent>(TEXT("ShopComponent"));

	bIsPlayerInRange = false;

	PrimaryActorTick.bCanEverTick = false;
}

void AShopTrigger::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = Cast<ACH8_GameState>(GetWorld()->GetGameState());
	SetupInputComponent();
}

void AShopTrigger::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (Cast<AMyCharacter>(OtherActor))	// 플레이어인지 확인
		{
			bIsPlayerInRange = true;
		}
	}
}

void AShopTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (Cast<AMyCharacter>(OtherActor))
		{
			bIsPlayerInRange = false;
		}
	}
}

void AShopTrigger::SetupInputComponent()
{
	// 플레이어 컨트롤러 가져오기
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		// Enhanced Input Subsystem 가져오기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Input Mapping Context 추가
			if (PlayerController->InputMappingContext)
			{
				Subsystem->AddMappingContext(PlayerController->InputMappingContext, 0);
			}
		}

		// Input Component 설정
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			if (PlayerController->ShopOpenAction)
			{
				EnhancedInputComponent->BindAction(
					PlayerController->ShopOpenAction,
					ETriggerEvent::Triggered,
					this,
					&AShopTrigger::OpenShop
				);
			}
		}
	}
}

void AShopTrigger::OpenShop()
{
	if (bIsPlayerInRange && ShopWidgetClass)
	{
		if (ShopWidgetClass)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			if (AMyCharacter* Player = Cast<AMyCharacter>(PlayerController->GetPawn()))
			{
				Player->ToggleInventory(NULL);
			}

			ShopWidgetInstance = CreateWidget<UUserWidget>(PlayerController, ShopWidgetClass);
			if (ShopWidgetInstance)
			{
				ShopWidgetInstance->AddToViewport();
				UpdateShopUI();

				// 마우스 커서 표시 및 입력 모드 변경
				PlayerController->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ShopWidgetInstance->TakeWidget());
				PlayerController->SetInputMode(InputMode);
			}

			AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (Player && Player->GetCharacterMovement())
			{
				Player->GetCharacterMovement()->StopMovementImmediately();
				Player->GetCharacterMovement()->SetMovementMode(MOVE_None);
			}
		}
	}
}

void AShopTrigger::UpdateShopUI()
{
	if (GameStateRef)
	{
		if (ShopWidgetInstance)
		{
			if (UTextBlock* GoldText = Cast<UTextBlock>(ShopWidgetInstance->GetWidgetFromName(TEXT("GoldText"))))
			{
				GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold : %d"), GameStateRef->GetGold())));
			}
		}
	}
}
