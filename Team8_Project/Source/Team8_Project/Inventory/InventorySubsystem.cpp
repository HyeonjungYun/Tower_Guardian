#include "InventorySubsystem.h"
#include "Algo/Sort.h"

UInventorySubsystem::UInventorySubsystem()
{
	ensure(MaxInventoryNum > 0);
	CurrentGold = 0;
	EquipmentItems.Init(FInventoryEquipment(), MaxInventoryNum);
	ConsumableItems.Init(FInventoryConsumable(), MaxInventoryNum);
	OthersItems.Init(FInventoryOthers(), MaxInventoryNum);
	AmmoItems.Init(FInventoryAmmo(), MaxInventoryNum);
}
int32 UInventorySubsystem::FindEquipmentIndex(const FName& ItemKey) const
{
	//TODO  : has vector Method Contain?
	for (int32 i = 0; i < EquipmentItems.Num(); i++)
	{
		if (EquipmentItems[i].ItemID == ItemKey)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventorySubsystem::FindConsumableIndex(const FName& ItemKey) const
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

int32 UInventorySubsystem::FindOthersIndex(const FName& ItemKey) const
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
int32 UInventorySubsystem::FindAmmoIndex(const FName& ItemKey) const
{
	for (int32 i = 0; i < AmmoItems.Num(); i++)
	{
		if (AmmoItems[i].ItemID == ItemKey)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
bool UInventorySubsystem::AddConsumableItem(const FName& ItemKey, int32 Quantity,UDataTable* SelectedDataTable)
{
	
	ensure(Quantity > 0);
	if (Quantity <= 0 || !SelectedDataTable)
	{
		return false;
	}

	//TODO : OthersItem
	//TODO : MaxInventoryNum
	const FConsumableItemRow* Row = SelectedDataTable->FindRow<FConsumableItemRow>(ItemKey, TEXT("LookupItemData"), true);

	if (Row->ItemType != EItemType::Consumable || !Row)
	{
		UE_LOG(LogTemp, Display, TEXT("im in SubSystem , Type Error"));
		return false;
	}

	int32 Index = FindConsumableIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		ConsumableItems[Index].Quantity += Quantity;
	}
	else
	{
		int32 EmptyIndex = INDEX_NONE;
		for (int32 i = 0; i < ConsumableItems.Num(); ++i)
		{
			if (ConsumableItems[i].ItemID.IsNone())
			{
				EmptyIndex = i;
				break;
			}
		}
		if (EmptyIndex != INDEX_NONE)
		{
			FInventoryConsumable NewConsumable;
			NewConsumable.ItemID = ItemKey;
			NewConsumable.ItemName = Row->ItemName;
			NewConsumable.Quantity = Quantity;
			NewConsumable.ItemImage = Row->ItemImage;
			ConsumableItems[EmptyIndex] = NewConsumable;
		}
		else if (EmptyIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Is Full"));
			return false;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=== Debug Print Consumable Items ==="));
	for (int32 i = 0; i < ConsumableItems.Num(); ++i)
	{
		const FInventoryConsumable& Item = ConsumableItems[i];
		UE_LOG(LogTemp, Log, TEXT("Index %d: ItemID: %s, ItemName: %s, Quantity: %d"),
			i,
			*Item.ItemID.ToString(),
			*Item.ItemName,
			Item.Quantity);
	}

	return true;
}
bool UInventorySubsystem::AddAmmoItem(const FName& ItemKey, int32 Quantity,UDataTable* SelectedDataTable)
{

	ensure(Quantity > 0);
	if (Quantity <= 0 || !SelectedDataTable)
	{
		return false;
	}

	//TODO : OthersItem
	//TODO : MaxInventoryNum
	const FWeaponAmmunitionRow* Row = SelectedDataTable->FindRow<FWeaponAmmunitionRow>(ItemKey, TEXT("LookupItemData"), true);

	if (Row->ItemType != EItemType::Ammo || !Row)
	{
		UE_LOG(LogTemp, Display, TEXT("im in SubSystem , Type Error"));
		return false;
	}

	int32 Index = FindAmmoIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		AmmoItems[Index].Quantity += Quantity;
	}
	else
	{
		int32 EmptyIndex = INDEX_NONE;
		for (int32 i = 0; i < AmmoItems.Num(); ++i)
		{
			if (AmmoItems[i].ItemID.IsNone())
			{
				EmptyIndex = i;
				break;
			}
		}
		if (EmptyIndex != INDEX_NONE)
		{
			FInventoryAmmo NewAmmo;
			NewAmmo.ItemID = ItemKey;
			NewAmmo.ItemName = Row->ItemName;
			NewAmmo.Quantity = Quantity;
			NewAmmo.ItemImage = Row->ItemImage;
			AmmoItems[EmptyIndex] = NewAmmo;
		}
		else if (EmptyIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Is Full"));
			return false;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=== Debug Print Ammo Items ==="));
	for (int32 i = 0; i < AmmoItems.Num(); ++i)
	{
		const FInventoryAmmo& Item = AmmoItems[i];
		UE_LOG(LogTemp, Log, TEXT("Index %d: ItemID: %s, ItemName: %s, Quantity: %d"),
			i,
			*Item.ItemID.ToString(),
			*Item.ItemName,
			Item.Quantity);
	}
	return true;
}



bool UInventorySubsystem::RemoveItem(const FName& ItemKey, int32 Quantity)
{
	int32 Index = FindConsumableIndex(ItemKey);
	//check -> quantity > ConsumableItems[Index].Quantity
	if (Index != INDEX_NONE)
	{
		if (ConsumableItems[Index].Quantity > Quantity)
		{
			ConsumableItems[Index].Quantity -= Quantity;
		}
		else
		{
			RemoveItemAt(Index, EItemType::Consumable);
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
			RemoveItemAt(Index, EItemType::Others);
		}
		return true;
	}

	Index = FindEquipmentIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		RemoveItemAt(Index, EItemType::Equipment);
		return true;
	}
	Index = FindAmmoIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		if (AmmoItems[Index].Quantity > Quantity)
		{
			AmmoItems[Index].Quantity -= Quantity;
		}
		else
		{
			RemoveItemAt(Index, EItemType::Ammo);
		}
		return true;
	}
	return false;
}
void UInventorySubsystem::RemoveItemAt(int32 SlotIndex, EItemType InventoryType)
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
	case EItemType::Ammo:
		if (AmmoItems.IsValidIndex(SlotIndex))
		{
			AmmoItems[SlotIndex] = FInventoryAmmo();
		}
		break;
	default:
		break;
	}
}
int32 UInventorySubsystem::GetGold() const
{
	return CurrentGold;
}
void UInventorySubsystem::SetGold(int32 NewGold)
{
	CurrentGold = NewGold;
}
void UInventorySubsystem::SortEquipmentItems(bool bIsAscending)
{
	Algo::Sort(EquipmentItems, [bIsAscending](const FInventoryEquipment& A, const FInventoryEquipment& B)
		{
			bool bIsAEmpty = A.ItemID.IsNone();
			bool bIsBEmpty = B.ItemID.IsNone();

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
void UInventorySubsystem::SortConsumableItems(bool bIsAscending)
{
	Algo::Sort(ConsumableItems, [bIsAscending](const FInventoryConsumable& A, const FInventoryConsumable& B)
		{
			bool bIsAEmpty = A.ItemID.IsNone();
			bool bIsBEmpty = B.ItemID.IsNone();

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
void UInventorySubsystem::SortOthersItems(bool bIsAscending)
{
	Algo::Sort(OthersItems, [bIsAscending](const FInventoryOthers& A, const FInventoryOthers& B)
		{
			bool bIsAEmpty = A.ItemID.IsNone();
			bool bIsBEmpty = B.ItemID.IsNone();

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
void UInventorySubsystem::SortAmmoItems(bool bIsAscending)
{
	Algo::Sort(AmmoItems, [bIsAscending](const FInventoryAmmo& A, const FInventoryAmmo& B)
		{
			bool bIsAEmpty = A.ItemID.IsNone();
			bool bIsBEmpty = B.ItemID.IsNone();

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
//return item ?
FName UInventorySubsystem::UseItem(int32 SlotIndex, EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		if (ConsumableItems.IsValidIndex(SlotIndex))
		{
			FInventoryConsumable& Item = ConsumableItems[SlotIndex];
			return Item.ItemID;
		}
		break;
	}
	case EItemType::Others:
	{
		if (OthersItems.IsValidIndex(SlotIndex))
		{
			FInventoryOthers& Item = OthersItems[SlotIndex];
			return Item.ItemID;
		}
		break;
	}
	case EItemType::Equipment:
	{
		if (OthersItems.IsValidIndex(SlotIndex))
		{
			FInventoryEquipment& Item = EquipmentItems[SlotIndex];
			return Item.ItemID;
		}
		break;
	}
	case EItemType::Ammo:
	{
		if (OthersItems.IsValidIndex(SlotIndex))
		{
			FInventoryAmmo& Item = AmmoItems[SlotIndex];
			return Item.ItemID;
		}
		break;
	}


	default:
		break;
	}
	return NAME_None;
}
const TArray<FInventoryConsumable>& UInventorySubsystem::GetConsumableItems() const
{
	return ConsumableItems;
}
const TArray<FInventoryEquipment>& UInventorySubsystem::GetEquipmentItems() const
{
	return EquipmentItems;
}
const TArray<FInventoryOthers>& UInventorySubsystem::GetOthersItems() const
{
	return OthersItems;
}
const TArray<FInventoryAmmo>& UInventorySubsystem::GetAmmoItems() const
{
	return AmmoItems;
}
void UInventorySubsystem::SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType)
{
	switch (CurrentSlotType)
	{
	case EItemType::Equipment:

		if (EquipmentItems.IsValidIndex(PrevIndex) && EquipmentItems.IsValidIndex(CurrentIndex))
		{
			EquipmentItems.Swap(PrevIndex,CurrentIndex);
		}
		break;

	case EItemType::Consumable:

		if (ConsumableItems.IsValidIndex(PrevIndex) && ConsumableItems.IsValidIndex(CurrentIndex))
		{
			ConsumableItems.Swap(PrevIndex,CurrentIndex);
		}
		break;

	case EItemType::Others:

		if (OthersItems.IsValidIndex(PrevIndex) && OthersItems.IsValidIndex(CurrentIndex))
		{
			OthersItems.Swap(PrevIndex, CurrentIndex);
		}
		break;
	case EItemType::Ammo:

		if (AmmoItems.IsValidIndex(PrevIndex) && AmmoItems.IsValidIndex(CurrentIndex))
		{
			AmmoItems.Swap(PrevIndex, CurrentIndex);
		}
		break;

	}


}
int32 UInventorySubsystem::SearchItemByName(const FName& ItemKey) const
{
	int32 Index = FindEquipmentIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		return EquipmentItems[Index].Quantity;
	}

	Index = FindConsumableIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		return ConsumableItems[Index].Quantity;
	}

	Index = FindOthersIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		return OthersItems[Index].Quantity;
	}

	Index = FindAmmoIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		return AmmoItems[Index].Quantity;
	}
	return INDEX_NONE;
}
int32 UInventorySubsystem::SearchItemByNameAndType(const FName& ItemKey, const EItemType& ItemType) const
{
	switch (ItemType)
	{
	case EItemType::Equipment:
	{
		int32 Index = FindEquipmentIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			return EquipmentItems[Index].Quantity;
		}
		return INDEX_NONE;
	}
	case EItemType::Consumable:
	{
		int32 Index = FindConsumableIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			return ConsumableItems[Index].Quantity;
		}
		return INDEX_NONE;
	}
	case EItemType::Others:
	{
		int32 Index = FindOthersIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			return OthersItems[Index].Quantity;
		}
		return INDEX_NONE;
	}
	case EItemType::Ammo:
	{
		int32 Index = FindAmmoIndex(ItemKey);
		if (Index != INDEX_NONE)
		{
			return AmmoItems[Index].Quantity;
		}
		return INDEX_NONE;
	}
	default:
		return INDEX_NONE;
	}
}
bool UInventorySubsystem::AddOthersItem(const FName& ItemKey, int32 Quantity, UDataTable* SelectedDataTable)
{

	ensure(Quantity > 0);
	if (Quantity <= 0 || !SelectedDataTable)
	{
		return false;
	}

	const FOtherItemRow* Row = SelectedDataTable->FindRow<FOtherItemRow>(ItemKey, TEXT("LookupOtherItemData"), true);

	if (Row->ItemType != EItemType::Others || !Row)
	{
		UE_LOG(LogTemp, Display, TEXT("im in SubSystem , Type Error"));
		return false;
	}

	int32 Index = FindOthersIndex(ItemKey);
	if (Index != INDEX_NONE)
	{
		OthersItems[Index].Quantity += Quantity;
	}
	else
	{
		int32 EmptyIndex = INDEX_NONE;
		for (int32 i = 0; i < OthersItems.Num(); ++i)
		{
			if (OthersItems[i].ItemID.IsNone())
			{
				EmptyIndex = i;
				break;
			}
		}
		if (EmptyIndex != INDEX_NONE)
		{
			FInventoryOthers NewItem;
			NewItem.ItemID = ItemKey;
			NewItem.ItemName = Row->ItemName;
			NewItem.Quantity = Quantity;
			NewItem.ItemImage = Row->ItemImage;
			OthersItems[EmptyIndex] = NewItem;
		}
		else if (EmptyIndex == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Is Full"));
			return false;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=== Debug Print Other Items ==="));
	for (int32 i = 0; i < OthersItems.Num(); ++i)
	{
		const FInventoryOthers& Item = OthersItems[i];
		UE_LOG(LogTemp, Log, TEXT("Index %d: ItemID: %s, ItemName: %s, Quantity: %d"),
			i,
			*Item.ItemID.ToString(),
			*Item.ItemName,
			Item.Quantity);
	}
	return true;
}
bool UInventorySubsystem::AddItem(const FName& ItemKey, int32 Quantity, EItemType ItemType, UDataTable* SelectedDataTable)
{
	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		return AddConsumableItem(ItemKey, Quantity, SelectedDataTable);
	}
	case EItemType::Others:
	{
		return AddOthersItem(ItemKey, Quantity, SelectedDataTable);
	}
	case EItemType::Ammo:
	{
		return AddAmmoItem(ItemKey, Quantity, SelectedDataTable);
	}
	case EItemType::Equipment:
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipment item addition is not supported yet."));
		return false;
	}
	default:
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown item type!"));
		return false;
	}
	}
}
//bool UInventorySubsystem::RemoveItemByNameAndType(const FName& ItemKey, int32 Quantity,EItemType ItemType)
// {}

