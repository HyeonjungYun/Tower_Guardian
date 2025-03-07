// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "InventoryType.h"
#include "Components/UniformGridPanel.h"
#include "SlotWidget.h"
#include "Components/UniformGridSlot.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UButton;
class UTextBlock;
class USlotWidget;
class IInventoryInterface;
class UUniformGridSlot;

UCLASS()
class TEAM8_PROJECT_API UInventoryWidget : public UCustomWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "InventoryUI")
	void InitInventoryUI();

	UFUNCTION(BlueprintCallable, Category = "InventoryUI")
	void UpdateInventoryUI();

	UFUNCTION(BlueprintCallable, Category = "InventoryUI")
	void OnSlotDoubleClicked(int32 SlotIndex, EItemType ItemType);

	UFUNCTION(BlueprintCallable, Category = "InventoryUI")
	void SetInventoryType(EItemType NewType);

	UFUNCTION(BlueprintCallable, Category = "InventoryUI")
	void SortCurrentItems();

	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void MoveStart();

	UFUNCTION()
	void MoveEnd();
private:
	UFUNCTION()
	void OnEquipmentTabClicked();

	UFUNCTION()
	void OnConsumableTabClicked();

	UFUNCTION()
	void OnOthersTabClicked();

	void InitInventorySlots();

	template<typename ItemType>
	void UpdatePanel(UUniformGridPanel* Panel, const TArray<ItemType>& Items);

	EItemType CurrentInventoryType;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryUI")
	TSubclassOf<USlotWidget> SlotWidgetClass;

	TScriptInterface<IInventoryInterface> InventoryInterface;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> EquipmentPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ConsumablePanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> OthersPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> AmmoPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquimentItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConsumableItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OtherItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AmmoItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SortItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

	TObjectPtr<class UBorder> Border;

	bool bIsDragging;
	FVector2D InitialOffset;
	FVector2D InitialPos;
	

	int32 FixedSlotCount = 30;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USlotWidget*> EquipmentSlots;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USlotWidget*> ConsumableSlots;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USlotWidget*> OthersSlots;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<USlotWidget*> AmmoSlots;

	UFUNCTION()
	void OnAmmoTabClicked();
};

template<typename ItemType>
void UInventoryWidget::UpdatePanel(UUniformGridPanel* Panel, const TArray<ItemType>& Items)
{
	if (!Panel || !SlotWidgetClass)
		return;

	Panel->ClearChildren();

	const int32 Columns = 5; 
	for (int32 Index = 0; Index < FixedSlotCount; Index++)
	{
		USlotWidget* NewSlot = CreateWidget<USlotWidget>(GetWorld(), SlotWidgetClass);
		
		if (NewSlot)
		{
			NewSlot->SetOwningActor(OwningActor);
			NewSlot->SlotIndex = Index;

			if (Panel == EquipmentPanel)
			{
				NewSlot->SetType(EItemType::Equipment);
			}
			else if (Panel == ConsumablePanel)
			{
				NewSlot->SetType(EItemType::Consumable);
			}
			else if (Panel == OthersPanel)
			{
				NewSlot->SetType(EItemType::Others);
			}
			else if (Panel == AmmoPanel)
			{
				NewSlot->SetType(EItemType::Ammo);
			}
			NewSlot->OnSlotDoubleClicked.AddDynamic(this, &UInventoryWidget::OnSlotDoubleClicked);
			NewSlot->InitializeSlot();

			UUniformGridSlot* GridSlot = Panel->AddChildToUniformGrid(NewSlot);
			if (GridSlot)
			{
				GridSlot->SetColumn(Index % Columns);
				GridSlot->SetRow(Index / Columns);
			}
		}
	}
}