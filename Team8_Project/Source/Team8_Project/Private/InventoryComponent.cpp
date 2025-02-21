#include "InventoryComponent.h"
#include "Algo/Sort.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentGold = 0;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UInventoryComponent::FindEquipmentIndex(const FName& ItemKey) const
{
	for (int32 i = 0; i < EquipmentItems.Num(); i++)
	{
		if (EquipmentItems[i].ItemID == ItemKey)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::FindConsumableIndex(const FName& ItemKey) const
{
	for (int32 i = 0; i < ConsumableItems.Num(); i++)
	{
		if (ConsumableItems[i].ItemID == ItemKey)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::FindOthersIndex(const FName& ItemKey) const
{
	for (int32 i = 0; i < OthersItems.Num(); i++)
	{
		if (OthersItems[i].ItemID == ItemKey)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

bool UInventoryComponent::AddItem(const FName& ItemKey, int32 Quantity)
{
	if (!ItemDataTable || Quantity <= 0)
	{
		return false;
	}
	const FConsumableItemRow* Row = ItemDataTable->FindRow<FConsumableItemRow>(ItemKey, TEXT("LookupItemData"), true);
	if (!Row)
	{
		return false;
	}

	switch (Row->ItemType)
	{
	case EItemType::Equipment:
	{
		int32 Index = FindEquipmentIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			EquipmentItems[Index].Quantity = 1;
		}
		else
		{
			FInventoryEquipment NewEquipment;
			NewEquipment.ItemID = ItemKey;
			NewEquipment.ItemName = Row->ItemName;
			NewEquipment.Quantity = 1;
			NewEquipment.ItemImage = Row->ItemImage;
			EquipmentItems.Add(NewEquipment);
		}
		break;
	}

	case EItemType::Consumable:
	{
		int32 Index = FindConsumableIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			ConsumableItems[Index].Quantity += Quantity;
		}
		else
		{
			FInventoryConsumable NewConsumable;
			NewConsumable.ItemID = ItemKey;
			NewConsumable.ItemName = Row->ItemName;
			NewConsumable.Quantity = Quantity;
			NewConsumable.ItemImage = Row->ItemImage;
			ConsumableItems.Add(NewConsumable);
			UE_LOG(LogTemp, Log, TEXT("Item In Inventory %s added to inventory."), *NewConsumable.ItemID.ToString());
			UE_LOG(LogTemp, Log, TEXT("ItemName In Inventory %s added to inventory."), *NewConsumable.ItemName);
		}
		break;
	}

	case EItemType::Others:
	{
		int32 Index = FindOthersIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			OthersItems[Index].Quantity += Quantity;
		}
		else
		{
			FInventoryOthers NewOthers;
			NewOthers.ItemID = ItemKey;
			NewOthers.ItemName = Row->ItemName;
			NewOthers.Quantity = Quantity;
			NewOthers.ItemImage = Row->ItemImage;
			OthersItems.Add(NewOthers);
		}
		break;
	}

	default:
		return false;
	}

	return true;
}

bool UInventoryComponent::RemoveItem(const FName& ItemKey, int32 Quantity)
{
	int32 Index = FindConsumableIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		if (ConsumableItems[Index].Quantity > Quantity)
		{
			ConsumableItems[Index].Quantity -= Quantity;
		}
		else
		{
			ConsumableItems.RemoveAt(Index);
		}
		return true;
	}

	Index = FindOthersIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		if (OthersItems[Index].Quantity > Quantity)
		{
			OthersItems[Index].Quantity -= Quantity;
		}
		else
		{
			OthersItems.RemoveAt(Index);
		}
		return true;
	}

	Index = FindEquipmentIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		EquipmentItems.RemoveAt(Index);
		return true;
	}

	return false;
}

void UInventoryComponent::RemoveItemAt(int32 SlotIndex, EItemType InventoryType)
{
	switch (InventoryType)
	{
	case EItemType::Equipment:
		if (EquipmentItems.IsValidIndex(SlotIndex))
		{
			EquipmentItems[SlotIndex] = FInventoryEquipment();
		}
		break;
	case EItemType::Consumable:
		if (ConsumableItems.IsValidIndex(SlotIndex))
		{
			ConsumableItems[SlotIndex] = FInventoryConsumable();
		}
		break;
	case EItemType::Others:
		if (OthersItems.IsValidIndex(SlotIndex))
		{
			OthersItems[SlotIndex] = FInventoryOthers();
		}
		break;
	default:
		break;
	}
}

int32 UInventoryComponent::GetGold() const
{
	return CurrentGold;
}
void UInventoryComponent::SetGold(int32 NewGold) 
{
	CurrentGold= NewGold;
}
void UInventoryComponent::SortEquipmentItems(bool bIsAscending)
{
	Algo::Sort(EquipmentItems, [bIsAscending](const FInventoryEquipment& A, const FInventoryEquipment& B)
		{
			bool bIsAEmpty = A.ItemName.IsEmpty();
			bool bIsBEmpty = B.ItemName.IsEmpty();

			if (bIsAEmpty && !bIsBEmpty)
			{
				return false;
			}
			else if (!bIsAEmpty && bIsBEmpty)
			{
				return true;
			}
			else if (bIsAEmpty && bIsBEmpty)
			{
				return false;
			}
			if (A.ItemName == B.ItemName)
			{
				return bIsAscending ? (A.Quantity < B.Quantity) : (A.Quantity > B.Quantity);
			}
			return bIsAscending ? (A.ItemName < B.ItemName) : (A.ItemName > B.ItemName);
		});
}

void UInventoryComponent::SortConsumableItems(bool bIsAscending)
{
	Algo::Sort(ConsumableItems, [bIsAscending](const FInventoryConsumable& A, const FInventoryConsumable& B)
		{
			bool bIsAEmpty = A.ItemName.IsEmpty();
			bool bIsBEmpty = B.ItemName.IsEmpty();

			if (bIsAEmpty && !bIsBEmpty)
			{
				return false;
			}
			else if (!bIsAEmpty && bIsBEmpty)
			{
				return true;
			}
			else if (bIsAEmpty && bIsBEmpty)
			{
				return false;
			}
			if (A.ItemName == B.ItemName)
			{
				return bIsAscending ? (A.Quantity < B.Quantity) : (A.Quantity > B.Quantity);
			}
			return bIsAscending ? (A.ItemName < B.ItemName) : (A.ItemName > B.ItemName);
		});
}

void UInventoryComponent::SortOthersItems(bool bIsAscending)
{
	Algo::Sort(OthersItems, [bIsAscending](const FInventoryOthers& A, const FInventoryOthers& B)
		{
			bool bIsAEmpty = A.ItemName.IsEmpty();
			bool bIsBEmpty = B.ItemName.IsEmpty();

			if (bIsAEmpty && !bIsBEmpty)
			{
				return false;
			}
			else if (!bIsAEmpty && bIsBEmpty)
			{
				return true;
			}
			else if (bIsAEmpty && bIsBEmpty)
			{
				return false;
			}
			if (A.ItemName == B.ItemName)
			{
				return bIsAscending ? (A.Quantity < B.Quantity) : (A.Quantity > B.Quantity);
			}
			return bIsAscending ? (A.ItemName < B.ItemName) : (A.ItemName > B.ItemName);
		});
}

bool UInventoryComponent::UseItem(int32 SlotIndex, EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		if (ConsumableItems.IsValidIndex(SlotIndex))
		{
			FInventoryConsumable& Item = ConsumableItems[SlotIndex];
			if (Item.Quantity > 0)
			{
				Item.Quantity--;
				// CharacterMethod
				UE_LOG(LogTemp, Warning, TEXT("Used Consumable: %s, Remaining: %d"), *Item.ItemName, Item.Quantity);
				if (Item.Quantity <= 0)
				{
					ConsumableItems[SlotIndex] = FInventoryConsumable();
				}
				return true;
			}
		}
		break;
	}
	//incomplete
	case EItemType::Others:
	{
		if (OthersItems.IsValidIndex(SlotIndex))
		{
			FInventoryOthers& Item = OthersItems[SlotIndex];
			if (Item.Quantity > 0)
			{
				Item.Quantity--;
				// CharacterMethod
				UE_LOG(LogTemp, Warning, TEXT("Used Others: %s, Remaining: %d"), *Item.ItemName, Item.Quantity);
				if (Item.Quantity <= 0)
				{
					OthersItems[SlotIndex] = FInventoryOthers();
				}
				return true;
			}
		}
		break;
	}
	//incomplete
	case EItemType::Equipment:
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipment item usage is not supported via UseItem."));
		break;
	}
	default:
		break;
	}
	return false;
}
