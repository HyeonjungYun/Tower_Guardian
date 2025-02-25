#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryType.h"
#include "DragSlot.generated.h"

UCLASS()
class TEAM8_PROJECT_API UDragSlot : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int PrevSlotIndex;

	UPROPERTY(VisibleAnywhere)
	EItemType SlotType;
};
