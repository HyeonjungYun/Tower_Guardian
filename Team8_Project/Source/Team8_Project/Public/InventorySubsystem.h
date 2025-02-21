// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryType.h"
#include "InventorySubsystem.generated.h"

UCLASS()
class TEAM8_PROJECT_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddConsumableItem(const FInventoryConsumable& NewItem);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddEquipmentItem(const FInventoryEquipment& NewItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddOtherItem(const FInventoryOthers& NewItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryConsumable>& GetConsumableItems() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryEquipment>& GetEquipmentItems() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FInventoryOthers>& GetOtherItems() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryConsumable> ConsumableItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEquipment> EquipmentItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryOthers> OtherItems;

};
