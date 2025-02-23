#include "InventoryComponent.h"
#include "InventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	//todo : has Component BeginPlay?
	if (GetWorld())
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			InventorySubsystem = GI->GetSubsystem<UInventorySubsystem>();
		}
	}
	//if (InventoryWidgetClass)
	//{
	//	InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
	//	if (InventoryWidget)
	//	{
	//		InventoryWidget->AddToViewport();
	//		UpdateInventoryUI();
	//	}
	//}
}

bool UInventoryComponent::AddItem(const FName& ItemKey, int32 Quantity)
{
	if (!ItemDataTable || Quantity <= 0)
	{
		return false;
	}
	//Add Something?

	if (InventorySubsystem)
	{
		//DataTable Reference?
		bool bResult = InventorySubsystem->AddItem(ItemKey, Quantity, ItemDataTable);
		if (bResult)
		{
			UpdateInventoryUI();
		}
		return bResult;
	}
	return false;

	
}

bool UInventoryComponent::RemoveItem(const FName& ItemKey, int32 Quantity)
{
	if (InventorySubsystem)
	{
		bool bResult = InventorySubsystem->RemoveItem(ItemKey, Quantity);
		if (bResult)
		{
			UpdateInventoryUI();
		}
		return bResult;
	}
	return false;
}

void UInventoryComponent::RemoveItemAt(int32 SlotIndex, EItemType InventoryType)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->RemoveItemAt(SlotIndex, InventoryType);
		UpdateInventoryUI();
	}
}

int32 UInventoryComponent::GetGold() const
{
	if (InventorySubsystem)
	{
		return InventorySubsystem->GetGold();
	}
	return 0;
}
void UInventoryComponent::SetGold(int32 NewGold) 
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SetGold(NewGold);
		UpdateInventoryUI();
	}
}
void UInventoryComponent::SortEquipmentItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortEquipmentItems(bIsAscending);
		UpdateInventoryUI();
	}
}

void UInventoryComponent::SortConsumableItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortConsumableItems(bIsAscending);
		UpdateInventoryUI();
	}
}

void UInventoryComponent::SortOthersItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortOthersItems(bIsAscending);
		UpdateInventoryUI();
	}
}

bool UInventoryComponent::UseItem(int32 SlotIndex, EItemType ItemType)
{
	//character has InventoryComponent
	if (InventorySubsystem)
	{
		bool bResult = InventorySubsystem->UseItem(SlotIndex, ItemType);
		if (bResult)
		{
			UpdateInventoryUI();
		}
		return bResult;
	}
	return false;
}
void UInventoryComponent::UpdateInventoryUI()
{
	UE_LOG(LogTemp, Log, TEXT("Inventory UI Updated."));
}