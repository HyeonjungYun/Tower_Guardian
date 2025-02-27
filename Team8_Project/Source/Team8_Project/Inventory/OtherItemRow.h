#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Inventory/InventoryType.h" 
#include "../BaseItem.h"
#include "OtherItemRow.generated.h"


USTRUCT(BlueprintType)
struct FOtherItemRow : public FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemKey = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Others;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemImage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ItemSalePrice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ItemPurchasePrice;
};
