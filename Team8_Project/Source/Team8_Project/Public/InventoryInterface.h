// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryType.h"
#include "InventoryInterface.generated.h"

/**
* 1.GetMethod => Go to InventorySubsystem
* 2.SetMethod => Go to InventoryComponent
*/

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM8_PROJECT_API IInventoryInterface
{
	GENERATED_BODY()

public:
	// Get Method
	virtual int32 GetGold() const = 0;
	virtual const TArray<FInventoryConsumable>& GetConsumableItems() const = 0;
	virtual const TArray<FInventoryEquipment>& GetEquipmentItems() const = 0;
	virtual const TArray<FInventoryOthers>& GetOthersItems() const = 0;

	// Set Method
	virtual bool AddItem(const FName& ItemKey, int32 Quantity) = 0;
	virtual bool RemoveItem(const FName& ItemKey, int32 Quantity) = 0;
	virtual bool UseItem(int32 SlotIndex, EItemType ItemType) = 0;
	virtual void SetGold(int32 NewGold) = 0;

	// Sort Method
	virtual void SortEquipmentItems(bool bIsAscending) = 0;
	virtual void SortConsumableItems(bool bIsAscending) = 0;
	virtual void SortOthersItems(bool bIsAscending) = 0;

};
