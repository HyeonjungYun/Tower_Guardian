#pragma once

#include "CoreMinimal.h"
#include "InventoryType.h"
#include "ConsumableItemRow.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class UInventorySubsystem;
class UCustomWidget;
class UInventoryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM8_PROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent();

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEquipment> EquipmentItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryConsumable> ConsumableItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryOthers> OthersItems;*/

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

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UDataTable* ItemDataTable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventorySubsystem* InventorySubsystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory|UI")
	TSubclassOf<UCustomWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|UI")
	UInventoryWidget* InventoryWidget = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Inventory|UI")
	void UpdateInventoryUI();


protected:
	virtual void BeginPlay() override;


};
