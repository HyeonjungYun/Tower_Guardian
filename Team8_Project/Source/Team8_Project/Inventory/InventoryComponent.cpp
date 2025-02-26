#include "InventoryComponent.h"
#include "InventorySubsystem.h"
#include "InventoryWidget.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			InventorySubsystem = GI->GetSubsystem<UInventorySubsystem>();
		}
	}
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->SetOwningActor(GetOwner());
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			UpdateInventoryUI();
		}
	}
}

bool UInventoryComponent::AddItem(const FName& ItemKey, int32 Quantity,EItemType ItemType)
{
	if (!ConsumableItemDataTable|| !AmmoItemDataTable || Quantity <= 0)
	{
		return false;
	}
	//Add Something?

	check(InventorySubsystem);
	bool bIsResult = SelectDataTableAdd(ItemKey, Quantity, ItemType);
	UpdateInventoryUI();

	return bIsResult;

}

bool UInventoryComponent::RemoveItem(const FName& ItemKey, int32 Quantity)
{
	if (InventorySubsystem)
	{
		bool bResult = InventorySubsystem->RemoveItem(ItemKey, Quantity);
		if (bResult)
		{
			UE_LOG(LogTemp, Warning, TEXT("im in Component Take a Key"));
			UpdateInventoryUI();
		}
		return bResult;
	}
	UE_LOG(LogTemp, Warning, TEXT("Im in Component , Fail to Remove"));
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
		//UpdateInventoryUI();
	}
}

void UInventoryComponent::SortConsumableItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortConsumableItems(bIsAscending);
		//UpdateInventoryUI();
	}
}

void UInventoryComponent::SortOthersItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortOthersItems(bIsAscending);
		//UpdateInventoryUI();
	}
}
void UInventoryComponent::SortAmmoItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortAmmoItems(bIsAscending);
		//UpdateInventoryUI();
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
			//UpdateInventoryUI();
		}
		return bResult;
	}
	return false;
}
void UInventoryComponent::UpdateInventoryUI()
{
	if (InventoryWidget)
	{
		InventoryWidget->UpdateInventoryUI();
	}
}
void UInventoryComponent::SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType)
{
	if (PrevSlotType == CurrentSlotType)
	{
		check(InventorySubsystem);
		InventorySubsystem->SwapItem(PrevIndex, CurrentIndex, PrevSlotType, CurrentSlotType);
		UpdateInventoryUI();
	}
}
bool UInventoryComponent::SelectDataTableAdd(const FName& ItemKey, int32 Quantity,const EItemType ItemType) const
{
	switch (ItemType)
	{
	case EItemType::Equipment:
	{

		//SelectedDataTable = 
		return false;
		break;
	}
	case EItemType::Consumable:
	{
		if (!ConsumableItemDataTable)
		{
			return false;
		}
		UDataTable* SelectedConsumableDataTable = ConsumableItemDataTable;
		bool bResult = InventorySubsystem->AddConsumableItem(ItemKey, Quantity, SelectedConsumableDataTable);
		
		return bResult;
		break;
	}
	case EItemType::Others:
	{
		if (!OtherItemDataTable)
		{
			return false;
		}
		UDataTable* SelectedConsumableDataTable = OtherItemDataTable;
		bool bResult = InventorySubsystem->AddOthersItem(ItemKey, Quantity, SelectedConsumableDataTable);

		return bResult;
		
		break;
	}
	case EItemType::Ammo:
	{
		if (!AmmoItemDataTable)
		{
			return false;
		}
		UDataTable* SelectedAmmoDataTable = AmmoItemDataTable;
		bool bResult = InventorySubsystem->AddAmmoItem(ItemKey, Quantity, SelectedAmmoDataTable);

		return bResult;
		break;
	}
	default:
		return false;
		break;
	}
}