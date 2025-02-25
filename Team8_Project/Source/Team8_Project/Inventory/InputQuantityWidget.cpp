#include "InputQuantityWidget.h"
#include "InventoryInterface.h"
#include "SlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void UInputQuantityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryInterface)
	{
		InventoryInterface = TScriptInterface<IInventoryInterface>(OwningActor);
	}

	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UInputQuantityWidget::HandleConfirmClicked);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInputQuantityWidget::HandleCloseClicked);
	}
	if (SlotRef)
	{
		SlotRef->OnSlotDragCancelled.AddDynamic(this, &UInputQuantityWidget::OnSlotDragCancelled);
	}
	

}

void UInputQuantityWidget::OnSlotDragCancelled(int32 SlotIndex, EItemType SlotType)
{
	SavedSlotIndex = SlotIndex;
	SavedSlotType = SlotType;
}

void UInputQuantityWidget::HandleConfirmClicked()
{
	if (QuantityTextBox)
	{
		FString Text = QuantityTextBox->GetText().ToString();
		int32 Quantity = FCString::Atoi(*Text);
		if (!Quantity)
		{
			//Do Something hmm...
		}
		HandleQuantity(SavedSlotIndex, SavedSlotType, Quantity);
	}
	RemoveFromParent();
}


void UInputQuantityWidget::HandleQuantity(int32 SlotIndex, EItemType SlotType, int32 Quantity)
{

	if (InventoryInterface)
	{
		FName ItemKey;
		switch (SlotType)
		{
		case EItemType::Equipment:
		{
			const TArray<FInventoryEquipment>& Items = InventoryInterface->GetEquipmentItems();
			if (Items.IsValidIndex(SlotIndex))
			{
				ItemKey = Items[SlotIndex].ItemID;
			}
			break;
		}
		case EItemType::Consumable:
		{
			const TArray<FInventoryConsumable>& Items = InventoryInterface->GetConsumableItems();
			if (Items.IsValidIndex(SlotIndex))
			{
				ItemKey = Items[SlotIndex].ItemID;
			}
			break;
		}
		case EItemType::Others:
		{
			const TArray<FInventoryOthers>& Items = InventoryInterface->GetOthersItems();
			if (Items.IsValidIndex(SlotIndex))
			{
				ItemKey = Items[SlotIndex].ItemID;
			}
			break;
		}
		default:
			break;
		}

		if (!ItemKey.IsNone())
		{
			InventoryInterface->RemoveItem(ItemKey, Quantity);
		}
	}
}

void UInputQuantityWidget::HandleCloseClicked()
{
	RemoveFromParent();
}