#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "InventoryType.h"
#include "SlotWidget.generated.h"


class UImage;
class UTextBlock;
class IInventoryInterface;


UCLASS()
class TEAM8_PROJECT_API USlotWidget : public UCustomWidget
{
	GENERATED_BODY()
public:
	void InitializeSlot();
	void SetType(EItemType InType);
	void UpdateSlot();

	UPROPERTY(VisibleAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	TObjectPtr<UImage> Image;

	UPROPERTY(VisibleAnywhere, Category = "Slot", meta = (BindWidget = "true"))
	TObjectPtr<UTextBlock> Quantity;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 SlotIndex;

	UPROPERTY(VisibleAnywhere, Category = "Slot")
	EItemType SlotType;

	
protected:
	/*virtual void NativeConstruct() override;*/
	void UpdateEquipmentSlot(IInventoryInterface* InventoryInterface);
	void UpdateConsumableSlot(IInventoryInterface* InventoryInterface);
	void UpdateOthersSlot(IInventoryInterface* InventoryInterface);

	UPROPERTY(EditAnywhere, Category = "Slot")
	TObjectPtr<class UTexture2D> DefaultTexture;
};
