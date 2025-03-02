#include "InventoryComponent.h"
#include "InventorySubsystem.h"
#include "InventoryWidget.h"
#include "ItemObject/ItemEffectBase.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			InventorySubsystem = GI->GetSubsystem<UInventorySubsystem>();
		}
	}
	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->SetOwningActor(GetOwner());
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			UpdateInventoryUI();
		}
	}
}

bool UInventoryComponent::AddItem(const FName& ItemKey, int32 Quantity,EItemType ItemType)
{
	if (!ConsumableItemDataTable|| !AmmoItemDataTable || Quantity <= 0)
	{
		return false;
	}
	check(InventorySubsystem);
	bool bIsResult = SelectDataTableAdd(ItemKey, Quantity, ItemType);
	UpdateInventoryUI();

	return bIsResult;

}

bool UInventoryComponent::RemoveItem(const FName& ItemKey, int32 Quantity)
{
	if (InventorySubsystem)
	{
		bool bResult = InventorySubsystem->RemoveItem(ItemKey, Quantity);
		if (bResult)
		{
			UpdateInventoryUI();
		}
		return bResult;
	}
	return false;
}

void UInventoryComponent::RemoveItemAt(int32 SlotIndex, EItemType InventoryType)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->RemoveItemAt(SlotIndex, InventoryType);
		UpdateInventoryUI();
	}
}

int32 UInventoryComponent::GetGold() const
{
	if (InventorySubsystem)
	{
		return InventorySubsystem->GetGold();
	}
	return 0;
}
void UInventoryComponent::SetGold(int32 NewGold) 
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SetGold(NewGold);
		UpdateInventoryUI();
	}
}
void UInventoryComponent::SortEquipmentItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortEquipmentItems(bIsAscending);
	}
}

void UInventoryComponent::SortConsumableItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortConsumableItems(bIsAscending);
	}
}

void UInventoryComponent::SortOthersItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortOthersItems(bIsAscending);
	}
}
void UInventoryComponent::SortAmmoItems(bool bIsAscending)
{
	if (InventorySubsystem)
	{
		InventorySubsystem->SortAmmoItems(bIsAscending);
	}
}
bool UInventoryComponent::UseItem(int32 SlotIndex, EItemType ItemType)
{
	if (InventorySubsystem)
	{
		bool bResult = InventorySubsystem->UseItem(SlotIndex, ItemType);
		if (bResult)
		{
			if (ItemType == EItemType::Consumable && ConsumableItemDataTable)
			{
				const TArray<FInventoryConsumable>& ConsumableItems = InventorySubsystem->GetConsumableItems();
				if (ConsumableItems.IsValidIndex(SlotIndex))
				{
					FName ItemKey = ConsumableItems[SlotIndex].ItemID;
					FConsumableItemRow* Row = ConsumableItemDataTable->FindRow<FConsumableItemRow>(ItemKey, TEXT("LookupItemData"), true);
					if (Row && Row->ItemEffectClass)
					{
						UItemEffectBase* Effect = NewObject<UItemEffectBase>(this, Row->ItemEffectClass);
						if (Effect)
						{
							Effect->ApplyItemEffect(GetOwner());
							UE_LOG(LogTemp, Warning, TEXT("Owner Use Potion"));
						}
					}
				}

				
			}
		}
	
		return bResult;
	}
	return false;
}
void UInventoryComponent::UpdateInventoryUI()
{
	if (InventoryWidget)
	{
		InventoryWidget->UpdateInventoryUI();
	}
}
void UInventoryComponent::SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType)
{
	if (PrevSlotType == CurrentSlotType)
	{
		ensure(InventorySubsystem);
		InventorySubsystem->SwapItem(PrevIndex, CurrentIndex, PrevSlotType, CurrentSlotType);
		UpdateInventoryUI();
	}
}
bool UInventoryComponent::SelectDataTableAdd(const FName& ItemKey, int32 Quantity,const EItemType ItemType) const
{
	UDataTable* SelectedDataTable = SelectDataTable(ItemKey,Quantity,ItemType);
	bool bResult = InventorySubsystem->AddItem(ItemKey, Quantity, ItemType, SelectedDataTable);
	return bResult;
}
UDataTable* UInventoryComponent::SelectDataTable(const FName& ItemKey, int32 Quantity, const EItemType ItemType) const
{
	UDataTable* SelectedDataTable = nullptr;
	switch (ItemType)
	{
	case EItemType::Equipment:
	{
		//No Settings
		return nullptr;
	}
	case EItemType::Consumable:
	{
		if (!ConsumableItemDataTable)
		{
			return nullptr;
		}
		SelectedDataTable = ConsumableItemDataTable;
		break;
	}
	case EItemType::Others:
	{
		if (!OtherItemDataTable)
		{
			return nullptr;
		}
		SelectedDataTable = OtherItemDataTable;
		break;
	}
	case EItemType::Ammo:
	{
		if (!AmmoItemDataTable)
		{
			return nullptr;
		}
		SelectedDataTable = AmmoItemDataTable;
		break;
	}
	default:
	{
		return nullptr;
	}
	}
	return SelectedDataTable;
}
int32 UInventoryComponent::ReturnAmmo(int32 RequiredAmmo, EWeaponType WeaponType)
{
	FName AmmoName = ReturnAmmoName(WeaponType);
	if (AmmoName.IsNone())
	{
		return -1;
	}
	int32 RemainAmmo = InventorySubsystem->SearchItemByNameAndType(AmmoName,EItemType::Ammo);
	if (RemainAmmo == INDEX_NONE)
	{
		return -1;
	}
	else if (RemainAmmo < RequiredAmmo) 
	{
		RemoveItem(AmmoName, RemainAmmo);
		return RemainAmmo;
	}

	else 
	{
		RemoveItem(AmmoName, RequiredAmmo);
		return RequiredAmmo;
	}



}
FName UInventoryComponent::ReturnAmmoName(EWeaponType WeaponType)
{
	if (!AmmoItemDataTable)
	{
		return NAME_None;
	}
	TArray<FName> RowNames = AmmoItemDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FWeaponAmmunitionRow* Row = AmmoItemDataTable->FindRow<FWeaponAmmunitionRow>(RowName, TEXT("LookupItemData"), true);
		if (Row && Row->WeaponType == WeaponType)
		{
			UE_LOG(LogTemp, Display, TEXT("Row->ItemKey %s"),*Row->ItemName);
			return Row->ItemKey;
		}
	}

	return NAME_None;
}