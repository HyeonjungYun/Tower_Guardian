#include "SlotWidget.h"
#include "InventoryInterface.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
	IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(OwningActor);
	if (!InventoryInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMMSlot::UpdateSlot - OwningActor does not implement IInventoryInterface."));
		return;
	}
	switch (SlotType)
	{
	case EItemType::Equipment:
		UpdateEquipmentSlot(InventoryInterface);
		break;
	case EItemType::Consumable:
		UpdateConsumableSlot(InventoryInterface);
		break;
	case EItemType::Others:
		UpdateOthersSlot(InventoryInterface);
		break;
	default:
		break;
	}
}
void USlotWidget::UpdateEquipmentSlot(IInventoryInterface* InventoryInterface)
{
	//Pass
}
void USlotWidget::UpdateConsumableSlot(IInventoryInterface* InventoryInterface)
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
void USlotWidget::UpdateOthersSlot(IInventoryInterface* InventoryInterface)
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
