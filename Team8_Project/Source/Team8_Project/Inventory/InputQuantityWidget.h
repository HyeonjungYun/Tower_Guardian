// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "InventoryType.h"
#include "InputQuantityWidget.generated.h"

class IInventoryInterface;

UCLASS()
class TEAM8_PROJECT_API UInputQuantityWidget : public UCustomWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* QuantityTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UFUNCTION(BlueprintCallable)
	void HandleConfirmClicked();

	UFUNCTION(BlueprintCallable)
	void HandleCloseClicked();

	UFUNCTION(BlueprintCallable, Category = "InventoryUI")
	void OnSlotDragCancelled(int32 SlotIndex, EItemType ItemType);

	UPROPERTY(BlueprintReadWrite, Category = "Slot")
	class USlotWidget* SlotRef;

protected:
	virtual void NativeConstruct() override;
private:
	void HandleQuantity(int32 SlotIndex, EItemType ItemType,int32 Quantity);
	TScriptInterface<IInventoryInterface> InventoryInterface;
	int32 SavedSlotIndex;
	EItemType SavedSlotType;
};
