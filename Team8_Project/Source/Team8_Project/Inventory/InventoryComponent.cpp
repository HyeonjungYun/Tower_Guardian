#include "InventoryComponent.h"
#include "InventorySubsystem.h"
#include "InventoryWidget.h"
#include "ItemObject/ItemEffectBase.h"
#include "Team8_Project/GameState/CH8_GameState.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "../Weapon/PlayerCombatComponent.h"
#include "../Weapon/WeaponType.h"
#include "../MyCharacter.h"
#include "../Weapon/AmmoType.h"
#include "../MyPlayerController.h"

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
	if (InventorySubsystem && AmmoItemDataTable)
	{
		FName Ammo = "RifleAmmo";
		InventorySubsystem->AddAmmoItem(Ammo, 100, AmmoItemDataTable);
		
		Ammo = "SniperAmmo";
		InventorySubsystem->AddAmmoItem(Ammo, 100, AmmoItemDataTable);
		
		Ammo = "ShotgunAmmo";
		InventorySubsystem->AddAmmoItem(Ammo, 100, AmmoItemDataTable);
		
		Ammo = "RocketAmmo";
		InventorySubsystem->AddAmmoItem(Ammo, 100, AmmoItemDataTable);
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

	// 탄약인경우 && 지금 들고있는 무기의 탄인 경우
	if (ItemType == EItemType::Ammo)
	{
		AMyCharacter* 
			OwnerPlayerCharacter = nullptr;
		OwnerPlayerCharacter = OwnerPlayerCharacter == nullptr ? Cast<AMyCharacter>(GetOwner()) : OwnerPlayerCharacter;
		// => UI 업데이트
		if (OwnerPlayerCharacter && OwnerPlayerCharacter->GetCombatComponent()->GetEquippedWeapon())
		{
			int32 CurWeaponAm = ReturnCurrentAmmo(OwnerPlayerCharacter->GetCombatComponent()
				->GetEquippedWeapon()->GetWeaponType());
			// 무기종류 : 탄종류 맵핑

			AMyPlayerController* OnwerPlayerController
				= nullptr;

			OnwerPlayerController
				= OnwerPlayerController == nullptr ? Cast<AMyPlayerController>(GetOwner()->GetInstigatorController()) : OnwerPlayerController;

			if (OnwerPlayerController)
			{
				CurWeaponAm = CurWeaponAm == -1 ? 0 : CurWeaponAm;
				OnwerPlayerController->SetHUDCarriedAmmo(CurWeaponAm);
			}
		}
	}


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
	if (UWorld* World = GetWorld())
	{
		if (ACH8_GameState* MyState = Cast<ACH8_GameState>(World->GetGameState()))
		{
			return MyState->GetGold();
		}
	}
	return 0;

	/*if (InventorySubsystem)
	{
		return InventorySubsystem->GetGold();
	}
	return 0;*/
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
	if (!InventorySubsystem)
	{
		return false;
	}

	switch (ItemType)
	{
	case EItemType::Consumable:
		return UseConsumableItem(SlotIndex);
	case EItemType::Equipment:
		return UseEquipmentItem(SlotIndex);
	case EItemType::Others:
		return UseOthersItem(SlotIndex);
	case EItemType::Ammo:
		return UseAmmoItem(SlotIndex);
	default:
		return false;
	}
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
	UDataTable* SelectedDataTable = SelectDataTable(ItemType);
	//ItemKey, Quantity,
	bool bResult = InventorySubsystem->AddItem(ItemKey, Quantity, ItemType, SelectedDataTable);
	return bResult;
}
UDataTable* UInventoryComponent::SelectDataTable(const EItemType ItemType) const
{
	//const FName& ItemKey, int32 Quantity,
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
	//인자하나 더 해서 함수 매개변수에 의한 값에의한 참조 깊은복사,얕은복사 
	// 남은 총 탄창을 Max를 
	FName AmmoName = ReturnAmmoName(WeaponType);
	if (AmmoName.IsNone())
	{
		return -1;
	}
	if (InventorySubsystem) 
	{
		
		int32 RemainAmmo = InventorySubsystem->SearchItemByNameAndType(AmmoName, EItemType::Ammo);
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
	return -1;
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
const TArray<FInventoryEquipment>& UInventoryComponent::GetEquipmentItems() const
{
	if (InventorySubsystem)
	{
		return InventorySubsystem->GetEquipmentItems();
	}
	static TArray<FInventoryEquipment> EmptyEquipments;
	return EmptyEquipments;
}
const TArray<FInventoryAmmo>& UInventoryComponent::GetAmmoItems() const
{
	if (InventorySubsystem)
	{
		return InventorySubsystem->GetAmmoItems();
	}
	static TArray<FInventoryAmmo> EmptyAmmos;
	return EmptyAmmos;
}
const TArray<FInventoryConsumable>& UInventoryComponent::GetConsumableItems() const
{
	if (InventorySubsystem)
	{
		return InventorySubsystem->GetConsumableItems();
	}
	static TArray<FInventoryConsumable> EmptyConsumables;
	return EmptyConsumables;
}
const TArray<FInventoryOthers>& UInventoryComponent::GetOthersItems() const
{
	if (InventorySubsystem)
	{
		return InventorySubsystem->GetOthersItems();
	}
	static TArray<FInventoryOthers> EmptyOthers;
	return EmptyOthers;
}
int32 UInventoryComponent::SearchItemByNameAndType(const FName& ItemKey, const EItemType& ItemType) const
{
	if (InventorySubsystem)
	{
		int32 Result = InventorySubsystem->SearchItemByNameAndType(ItemKey, ItemType);
		return Result;
	}
	return INDEX_NONE;
}
int32 UInventoryComponent::SearchItemByName(const FName& ItemKey) const
{
	if (InventorySubsystem)
	{
		int32 Result = InventorySubsystem->SearchItemByName(ItemKey);
		return Result;
	}
	return INDEX_NONE;
}
int32 UInventoryComponent::ReturnCurrentAmmo(EWeaponType WeaponType) 
{
	const FName& AmmoName = ReturnAmmoName(WeaponType);
	if (AmmoName.IsNone())
	{
		return INDEX_NONE;
	}
	if (InventorySubsystem) 
	{
		int32 RemainAmmo = InventorySubsystem->SearchItemByNameAndType(AmmoName, EItemType::Ammo);
		return RemainAmmo;
	}
	
	return INDEX_NONE;
}
bool UInventoryComponent::UseConsumableItem(int32 SlotIndex)
{
	FName NameKey = InventorySubsystem->UseItem(SlotIndex, EItemType::Consumable);
	if (!NameKey.IsNone())
	{
		FConsumableItemRow* Row = ConsumableItemDataTable->FindRow<FConsumableItemRow>(NameKey, TEXT("LookupItemData"), true);
		if (Row && Row->ItemEffectClass)
		{
			UItemEffectBase* Effect = NewObject<UItemEffectBase>(this, Row->ItemEffectClass);
			if (Effect)
			{
				Effect->ApplyItemEffect(GetOwner());
				UE_LOG(LogTemp, Warning, TEXT("Owner used Consumable Item: %s"), *NameKey.ToString());
				return InventorySubsystem->RemoveItem(NameKey, 1);
			}
		}
	}
	return false;
}
bool UInventoryComponent::UseEquipmentItem(int32 SlotIndex)
{
	
	FName NameKey = InventorySubsystem->UseItem(SlotIndex, EItemType::Equipment);
	if (!NameKey.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dont Use"));
		return true;
	}
	return false;
}
bool UInventoryComponent::UseOthersItem(int32 SlotIndex)
{
	FName NameKey = InventorySubsystem->UseItem(SlotIndex, EItemType::Others);
	if (!NameKey.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dont Use"));
		return true;
	}
	return false;
}
bool UInventoryComponent::UseAmmoItem(int32 SlotIndex)
{
	FName NameKey = InventorySubsystem->UseItem(SlotIndex, EItemType::Ammo);
	if (!NameKey.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dont Use"));
		return true;
	}
	return false;
}