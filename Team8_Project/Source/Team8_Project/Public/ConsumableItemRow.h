#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryType.h" 
#include "ConsumableItemRow.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
    Buff,
    Debuff,
    None
};

USTRUCT(BlueprintType)
struct FConsumableItemRow : public FTableRowBase
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemKey = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Consumable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemImage = nullptr;

    UPROPERTY(EditAnywhere, Category = "Item")
    EBuffType BuffType;

    UPROPERTY(EditAnywhere, Category = "Item")
    float Amount;

    UPROPERTY(EditAnywhere, Category = "Item")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ItemSalePrice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ItemPurchasePrice;
   
};
