#include "ShopComponent.h"
#include "ShopTrigger.h"
#include "Team8_Project/GameState/CH8_GameState.h"

UShopComponent::UShopComponent() 
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShopComponent::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = Cast<ACH8_GameState>(GetWorld()->GetGameState());
}

void UShopComponent::AttemptItemPurchase()
{
	int ItemCost = 100;

	if (GameStateRef)
	{
		if (GameStateRef->GetGold() >= ItemCost)
		{
			if (GameStateRef->OnGameSetGold.IsBound())
			{
				GameStateRef->OnGameSetGold.Broadcast(-ItemCost);
				Cast<AShopTrigger>(GetOwner())->UpdateShopUI();
			}
		}
	}
}

