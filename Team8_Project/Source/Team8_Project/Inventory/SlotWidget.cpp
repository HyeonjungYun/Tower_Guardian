#include "SlotWidget.h"
#include "InventoryInterface.h"
#include "DragSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
void USlotWidget::InitializeSlot()
{
	UpdateSlot();
}
void USlotWidget::SetType(EItemType InType)
{
	SlotType = InType;
}
void USlotWidget::UpdateSlot()
{
	InventoryInterface = TScriptInterface<IInventoryInterface>(OwningActor);
	if (!InventoryInterface)
	{
		return;
	}
	switch (SlotType)
	{
	case EItemType::Equipment:
		UpdateEquipmentSlot();
		break;
	case EItemType::Consumable:
		UpdateConsumableSlot();
		break;
	case EItemType::Others:
		UpdateOthersSlot();
		break;
	default:
		break;
	}
}
void USlotWidget::UpdateEquipmentSlot()
{
	Image->SetBrushFromTexture(DefaultTexture);
	Quantity->SetText(FText::FromString(TEXT("")));

}
void USlotWidget::UpdateConsumableSlot()
{
	const TArray<FInventoryConsumable>& ConsumableItems = InventoryInterface->GetConsumableItems();
	if (ConsumableItems.IsValidIndex(SlotIndex))
	{
		const FInventoryConsumable& Item = ConsumableItems[SlotIndex];
		if (!Item.ItemName.IsEmpty() && Item.Quantity > 0)
		{
			Image->SetBrushFromTexture(Item.ItemImage);
			Quantity->SetText(FText::FromString(FString::Printf(TEXT("%d"),Item.Quantity)));
		}
		else
		{
			Image->SetBrushFromTexture(DefaultTexture);
			Quantity->SetText(FText::FromString(TEXT("")));
		}
	}
}
void USlotWidget::UpdateOthersSlot()
{
	const TArray<FInventoryOthers>& OtherItems = InventoryInterface->GetOthersItems();
	if (OtherItems.IsValidIndex(SlotIndex))
	{
		const FInventoryOthers& Item = OtherItems[SlotIndex];
		if (!Item.ItemName.IsEmpty() && Item.Quantity > 0)
		{
			Image->SetBrushFromTexture(Item.ItemImage);
			Quantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), Item.Quantity)));
		}
		else
		{
			Image->SetBrushFromTexture(DefaultTexture);
			Quantity->SetText(FText::FromString(TEXT("")));
		}
	}
}
FReply USlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (OnSlotDoubleClicked.IsBound())
	{
		OnSlotDoubleClicked.Broadcast(SlotIndex, SlotType);
	}
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}


FReply USlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//Left Mouse Input
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		bool bIsSuccess = false;
		if (InventoryInterface)
		{
			
			switch (SlotType)
			{
				
			case EItemType::Equipment:
				if (InventoryInterface->GetEquipmentItems().IsValidIndex(SlotIndex))
				{
					bIsSuccess = true;
				}
				break;
			case EItemType::Consumable:
				if (InventoryInterface->GetConsumableItems().IsValidIndex(SlotIndex))
				{
					bIsSuccess = true;
				}
				break;
			case EItemType::Others:
				if (InventoryInterface->GetOthersItems().IsValidIndex(SlotIndex))
				{
					bIsSuccess = true;
				}
				break;
			}

			if (bIsSuccess)
			{
				Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			}
		}
	}

	return Reply.NativeReply;
}

void USlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UDragSlot* Operation = NewObject<UDragSlot>();
		OutOperation = Operation;

		Operation->PrevSlotIndex = SlotIndex;
		Operation->SlotType = SlotType;

		if (DragWidgetClass)
		{
			USlotWidget* DragWidget = CreateWidget<USlotWidget>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				DragWidget->SlotType = SlotType;
				DragWidget->SetOwningActor(OwningActor);
				DragWidget->SlotIndex = SlotIndex;
				DragWidget->InitializeSlot();

				Operation->DefaultDragVisual = DragWidget;
			}
		}
	}
}
//드래그된 아이템이 현재 슬롯 위에 떨어졌을 때 호출되는 함수
bool USlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragSlot* Operation = Cast<UDragSlot>(InOperation);

	if (Operation)
	{
		if (Operation->SlotType == SlotType)
		{
			if (InventoryInterface)
			{
				InventoryInterface->SwapItem(Operation->PrevSlotIndex, SlotIndex, Operation->SlotType, SlotType);
			}
			return true;
		}
	}
	return false;
}
void USlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	OnSlotDragCancelled.Broadcast(SlotIndex, SlotType);
	QuantityInputHUD();
	//
}


void USlotWidget::QuantityInputHUD()
{
	/*ensure(!QuantityInputWidgetClass);
	*/
	QuantityInputWidget = CreateWidget<UUserWidget>(GetWorld(),QuantityInputWidgetClass);

	/*check(!QuantityInputWidget)*/;
	if (QuantityInputWidget) 
	{
		QuantityInputWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("InputQuantityHUD displayed."));
	}
}
