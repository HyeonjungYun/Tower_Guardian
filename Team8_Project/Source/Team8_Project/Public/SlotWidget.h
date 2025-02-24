#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "InventoryType.h"
#include "SlotWidget.generated.h"


class UImage;
class UTextBlock;
class IInventoryInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotDoubleClicked, int32, SlotIndex, EItemType, ItemType);


UCLASS()
class TEAM8_PROJECT_API USlotWidget : public UCustomWidget
{
	GENERATED_BODY()
public:
	void InitializeSlot();
	void SetType(EItemType InType);
	void UpdateSlot();

	UPROPERTY(BlueprintAssignable, Category = "Slot")
	FOnSlotDoubleClicked OnSlotDoubleClicked;

	UPROPERTY(VisibleAnywhere, Category = "Slot", meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(VisibleAnywhere, Category = "Slot", meta = (BindWidget))
	TObjectPtr<UTextBlock> Quantity;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, Category = "Slot")
	EItemType SlotType;


protected:

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	void UpdateEquipmentSlot();
	void UpdateConsumableSlot();
	void UpdateOthersSlot();

	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTexture2D> DefaultTexture;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<USlotWidget> DragWidgetClass;
private:
	TScriptInterface<IInventoryInterface> InventoryInterface;
};
