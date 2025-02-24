#include "SlotWidget.h"
#include "InventoryInterface.h"
#include "DragSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
		UE_LOG(LogTemp, Warning, TEXT("im in slot No Owning Actor"));
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
		UE_LOG(LogTemp, Warning, TEXT("im in Slot Start Consumable Update"));
		const FInventoryConsumable& Item = ConsumableItems[SlotIndex];
		if (!Item.ItemName.IsEmpty() && Item.Quantity > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("im in Slot Start Image And Quantity Start"));
			Image->SetBrushFromTexture(Item.ItemImage);
			Quantity->SetText(FText::FromString(FString::Printf(TEXT("%d"),Item.Quantity)));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("im in Slot No Image And Quantity Start"));
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