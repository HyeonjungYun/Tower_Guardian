// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipment   UMETA(DisplayName = "Equipment"),
	Consumable  UMETA(DisplayName = "Consumable")
};

USTRUCT(BlueprintType)
struct FInventoryConsumable
{
	GENERATED_BODY()

	// 아이템 식별용 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;

	// 아이템 이름 (UI 표시용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemName;

	// 아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	// 아이템 아이콘 (UTexture2D 에셋)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemImage;

	FInventoryConsumable()
		: ItemID(NAME_None), ItemName(TEXT("")), Quantity(0), ItemImage(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FInventoryEquipment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemName;

	// 일반적으로 장비는 1개지만 확장을 위해 수량을 둠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemImage;

	FInventoryEquipment()
		: ItemID(NAME_None), ItemName(TEXT("")), Quantity(0), ItemImage(nullptr)
	{
	}
};
