#include "InventorySubsystem.h"

void UInventorySubsystem::AddConsumableItem(const FInventoryConsumable& NewItem)
{
	FInventoryConsumable* ExistingItem = ConsumableItems.FindByPredicate([&](const FInventoryConsumable& Item)
		{
			return Item.ItemID == NewItem.ItemID;
		});

	if (ExistingItem)
	{
		ExistingItem->Quantity += NewItem.Quantity;
	}
	else
	{
		ConsumableItems.Add(NewItem);
	}
}

void UInventorySubsystem::AddEquipmentItem(const FInventoryEquipment& NewItem)
{
	FInventoryEquipment* ExistingItem = EquipmentItems.FindByPredicate([&](const FInventoryEquipment& Item)
		{
			return Item.ItemID == NewItem.ItemID;
		});

	if (ExistingItem)
	{
		ExistingItem->Quantity += NewItem.Quantity;
	}
	else
	{
		EquipmentItems.Add(NewItem);
	}
}
void UInventorySubsystem::AddOtherItem(const FInventoryOthers& NewItem)
{
	FInventoryOthers* ExistingItem = OtherItems.FindByPredicate([&](const FInventoryOthers& Item)
		{
			return Item.ItemID == NewItem.ItemID;
		});

	if (ExistingItem)
	{
		ExistingItem->Quantity += NewItem.Quantity;
	}
	else
	{
		OtherItems.Add(NewItem);
	}
}

const TArray<FInventoryConsumable>& UInventorySubsystem::GetConsumableItems() const
{
	return ConsumableItems;
}

const TArray<FInventoryEquipment>& UInventorySubsystem::GetEquipmentItems() const
{
	return EquipmentItems;
}
const TArray<FInventoryOthers>& UInventorySubsystem::GetOtherItems() const
{
	return OtherItems;
}