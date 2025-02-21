﻿#include "ShopTrigger.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Team8_Project/MyCharacter.h"
#include "Kismet/GameplayStatics.h"

AShopTrigger::AShopTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(100.0f));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AShopTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AShopTrigger::OnOverlapEnd);

	bIsPlayerInRange = false;

	PrimaryActorTick.bCanEverTick = false;
}

void AShopTrigger::BeginPlay()
{
	Super::BeginPlay();
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
	if (bIsPlayerInRange && ShopWidgetClass)
	{
		UE_LOG(LogTemp, Log, TEXT("범위 내에서 F키 누름"));
		
		if (ShopWidgetClass)
		{
			UE_LOG(LogTemp, Log, TEXT("상점 오픈 시작"));
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			ShopWidgetInstance = CreateWidget<UUserWidget>(PlayerController, ShopWidgetClass);
			if (ShopWidgetInstance)
			{
				UE_LOG(LogTemp, Log, TEXT("상점 오픈"));
				ShopWidgetInstance->AddToViewport();

				// 마우스 커서 표시 및 입력 모드 변경
				PlayerController->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ShopWidgetInstance->TakeWidget());
				PlayerController->SetInputMode(InputMode);
			}
		}
	}
}
