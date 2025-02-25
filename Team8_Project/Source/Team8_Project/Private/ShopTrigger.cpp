#include "ShopTrigger.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/Public/ShopComponent.h"
#include "Team8_Project/Public/CH8_GameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		InputComponent = NewObject<UInputComponent>(this);
		InputComponent->RegisterComponent();

		if (InputComponent)
		{
			InputComponent->BindAction("OpenShop", IE_Pressed, this, &AShopTrigger::OpenShop);
			PlayerController->PushInputComponent(InputComponent);
		}
	}
}

void AShopTrigger::OpenShop()
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player && Player->GetCharacterMovement())
	{
		//Player->GetCharacterMovement()->StopMovementImmediately();
		//Player->GetCharacterMovement()->SetMovementMode(MOVE_None);
	}

	if (bIsPlayerInRange && ShopWidgetClass)
	{
		
		if (ShopWidgetClass)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

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
