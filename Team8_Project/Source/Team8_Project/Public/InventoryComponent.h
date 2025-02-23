#pragma once

#include "CoreMinimal.h"
#include "InventoryType.h"
#include "ItemData.h"
#include "ConsumableItemRow.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// delete ItemData.h

class UInventorySubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEquipment> EquipmentItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryConsumable> ConsumableItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryOthers> OthersItems;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetGold(int32 NewGold);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName& ItemKey, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(const FName& ItemKey, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemAt(int32 InSlotIndex, EItemType InventoryType);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortEquipmentItems(bool bIsAscending);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortConsumableItems(bool bIsAscending);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortOthersItems(bool bIsAscending);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 SlotIndex, EItemType ItemType);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UDataTable* ItemDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventorySubsystem* InventorySubsystem = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|UI")
	//TSubclassOf<UUserWidget> InventoryWidgetClass;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|UI")
	//UUserWidget* InventoryWidget = nullptr;
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateInventoryUI();


protected:
	virtual void BeginPlay() override;


};
