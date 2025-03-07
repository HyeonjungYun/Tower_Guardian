#include "InventoryWidget.h"
#include "SlotWidget.h"
#include "InventoryInterface.h"
#include "InventorySubsystem.h"
#include "InventoryComponent.h"
#include "Team8_Project/GameState/CH8_GameState.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"   
#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"  
#include "Blueprint/WidgetLayoutLibrary.h"



void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryInterface = TScriptInterface<IInventoryInterface>(OwningActor);

	if (!InventoryInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Owning Actor"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("im in InvenWiget has a Owning Actor"));

	CurrentInventoryType = EItemType::Equipment;

	if (EquimentItemButton)
	{
		EquimentItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnEquipmentTabClicked);
	}
	if (ConsumableItemButton)
	{
		ConsumableItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnConsumableTabClicked);
	}
	if (OtherItemButton)
	{
		OtherItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnOthersTabClicked);
	}
	if (SortItemButton)
	{
		SortItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::SortCurrentItems);
	}
	if (MainButton)
	{
		MainButton->OnPressed.AddDynamic(this, &UInventoryWidget::MoveStart);
		MainButton->OnReleased.AddDynamic(this, &UInventoryWidget::MoveEnd);
	}
	if (AmmoItemButton)
	{
		AmmoItemButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnAmmoTabClicked);
	}

	if (UWorld* World = GetWorld())
	{
		// GameState를 가져오고, ACH8_GameState로 캐스팅
		ACH8_GameState* MyGameState = Cast<ACH8_GameState>(World->GetGameState());
		if (MyGameState)
		{
			MyGameState->OnGoldChanged.AddDynamic(this, &UInventoryWidget::UpdateInventoryUI);
		}
	}
	InitInventorySlots();
	InitInventoryUI();
}
void UInventoryWidget::InitInventorySlots()
{
	bIsDragging = false;
	TArray<UWidget*> TempWidgets;
	WidgetTree->GetAllWidgets(TempWidgets);
	for (UWidget* widget : TempWidgets)
	{
		UBorder* Findedborder = Cast<UBorder>(widget);
		if (Findedborder)
		{
			Border = Findedborder;
			break;
		}
	}
	EquipmentSlots.Init(nullptr, FixedSlotCount);
	ConsumableSlots.Init(nullptr, FixedSlotCount);
	OthersSlots.Init(nullptr, FixedSlotCount);
	AmmoSlots.Init(nullptr, FixedSlotCount);
}
void UInventoryWidget::InitInventoryUI()
{
	if (EquipmentPanel)
	{
		EquipmentPanel->ClearChildren();
		UpdatePanel(EquipmentPanel, InventoryInterface->GetEquipmentItems());
		EquipmentPanel->SetVisibility(ESlateVisibility::Visible);
	}
	if (ConsumablePanel)
	{
		ConsumablePanel->ClearChildren();
		UpdatePanel(ConsumablePanel, InventoryInterface->GetConsumableItems());
		ConsumablePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (OthersPanel)
	{
		OthersPanel->ClearChildren();
		UpdatePanel(OthersPanel, InventoryInterface->GetOthersItems());
		OthersPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (AmmoPanel)
	{
		AmmoPanel->ClearChildren();
		UpdatePanel(AmmoPanel, InventoryInterface->GetAmmoItems());
		AmmoPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(InventoryInterface->GetGold()));
	}
	UpdateInventoryUI();
}
void UInventoryWidget::UpdateInventoryUI()
{
	if (!InventoryInterface)
	{
		return;
	}
	switch (CurrentInventoryType)
	{
	case EItemType::Equipment:
		if (EquipmentPanel)
		{
			UpdatePanel(EquipmentPanel, InventoryInterface->GetEquipmentItems());
		}
		break;
	case EItemType::Consumable:
		if (ConsumablePanel)
		{
			UpdatePanel(ConsumablePanel, InventoryInterface->GetConsumableItems());
		}
		break;
	case EItemType::Others:
		if (OthersPanel)
		{
			UpdatePanel(OthersPanel, InventoryInterface->GetOthersItems());
		}
		break;
	case EItemType::Ammo:
		if (AmmoPanel)
		{
			UpdatePanel(AmmoPanel, InventoryInterface->GetAmmoItems());
		}
		break;
	default:
		break;
	}
	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(InventoryInterface->GetGold()));
	}
}
void UInventoryWidget::OnSlotDoubleClicked(int32 SlotIndex, EItemType ItemType)
{
	if (!InventoryInterface)
	{
		return;
	}
	bool bIsUsed = InventoryInterface->UseItem(SlotIndex, ItemType);
	if (bIsUsed)
	{
		UpdateInventoryUI();
	}
}
void UInventoryWidget::SetInventoryType(EItemType NewType)
{
	//위에 판넬 if문 추가 , 밑에 주석
	CurrentInventoryType = NewType;


	if (EquipmentPanel)
	{
		EquipmentPanel->ClearChildren();
		EquipmentPanel->SetVisibility(CurrentInventoryType == EItemType::Equipment ?
			ESlateVisibility::Visible :
			ESlateVisibility::Collapsed);
	}
	if (ConsumablePanel)
	{
		ConsumablePanel->ClearChildren();
		ConsumablePanel->SetVisibility(CurrentInventoryType == EItemType::Consumable ?
			ESlateVisibility::Visible :
			ESlateVisibility::Collapsed);
	}
	if (OthersPanel)
	{
		OthersPanel->ClearChildren();
		OthersPanel->SetVisibility(CurrentInventoryType == EItemType::Others ?
			ESlateVisibility::Visible :
			ESlateVisibility::Collapsed);
	}
	if (AmmoPanel)
	{
		AmmoPanel->ClearChildren();
		AmmoPanel->SetVisibility(CurrentInventoryType == EItemType::Ammo ?
			ESlateVisibility::Visible :
			ESlateVisibility::Collapsed);
	}
	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(InventoryInterface->GetGold()));
	}


	UpdateInventoryUI();
}
void UInventoryWidget::OnEquipmentTabClicked()
{
	SetInventoryType(EItemType::Equipment);
}

void UInventoryWidget::OnConsumableTabClicked()
{
	SetInventoryType(EItemType::Consumable);
}

void UInventoryWidget::OnOthersTabClicked()
{
	SetInventoryType(EItemType::Others);
}
void UInventoryWidget::SortCurrentItems()
{
	if (!InventoryInterface)
	{
		return;
	}
	switch (CurrentInventoryType)
	{
	case EItemType::Equipment:
		InventoryInterface->SortEquipmentItems(false);
		UpdateInventoryUI();
		break;
	case EItemType::Consumable:
		InventoryInterface->SortConsumableItems(false);
		UpdateInventoryUI();
		break;
	case EItemType::Others:
		InventoryInterface->SortOthersItems(false);
		UpdateInventoryUI();
		break;
	case EItemType::Ammo:
		InventoryInterface->SortAmmoItems(false);
		UpdateInventoryUI();
		break;
	default:
		break;
	}
}
void UInventoryWidget::MoveStart()
{
	bIsDragging = true;

	//Get Boarder Position
	FVector2D WidgetPos;
	UCanvasPanelSlot* Borderslot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Border);
	if (Borderslot)
	{
		WidgetPos = Borderslot->GetPosition();
	}
	InitialPos = WidgetPos;

	//Get Mouse Position
	InitialOffset = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
}

void UInventoryWidget::MoveEnd()
{
	bIsDragging = false;
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsDragging)
	{
		FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		float DeltaX = InitialOffset.X - MousePos.X;
		float DeltaY = InitialOffset.Y - MousePos.Y;

		InitialPos.X += -DeltaX;
		InitialPos.Y += -DeltaY;

		InitialOffset = MousePos;

		//Set Boarder Posiong
		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Border);
		if (slot)
		{
			slot->SetPosition(InitialPos);
		}
	}
}
void UInventoryWidget::OnAmmoTabClicked()
{
	SetInventoryType(EItemType::Ammo);
}