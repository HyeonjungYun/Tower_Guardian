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

bool UShopComponent::AttemptItemPurchase(int32 ItemCost)
{
	if (GameStateRef)
	{
		if (GameStateRef->GetGold() >= ItemCost)
		{
			GameStateRef->SetGold(-ItemCost);
			Cast<AShopTrigger>(GetOwner())->UpdateShopUI();

			return true;
		}
	}

	return false;
}

