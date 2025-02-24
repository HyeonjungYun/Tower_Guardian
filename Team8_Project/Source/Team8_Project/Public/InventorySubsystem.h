// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "InventoryType.h"
#include "ConsumableItemRow.h"  
#include "InventorySubsystem.generated.h"

UCLASS()
class TEAM8_PROJECT_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//TODO : IF Save File , Setting Initialize
	UInventorySubsystem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName& ItemKey, int32 Quantity, UDataTable* ItemDataTable);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(const FName& ItemKey, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemAt(int32 SlotIndex, EItemType InventoryType);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortEquipmentItems(bool bIsAscending);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortConsumableItems(bool bIsAscending);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortOthersItems(bool bIsAscending);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 SlotIndex, EItemType ItemType);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapItem(int32 PrevIndex,int32 CurrentIndex,EItemType PrevSlotType, EItemType CurrentSlotType);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetGold(int32 NewGold);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryConsumable>& GetConsumableItems() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryEquipment>& GetEquipmentItems() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryOthers>& GetOthersItems() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryConsumable> ConsumableItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEquipment> EquipmentItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryOthers> OthersItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 CurrentGold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxInventoryNum = 30;

private:
	int32 FindEquipmentIndex(const FName& ItemKey) const;

	int32 FindConsumableIndex(const FName& ItemKey) const;

	int32 FindOthersIndex(const FName& ItemKey) const;
};
