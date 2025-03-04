// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equipment   UMETA(DisplayName = "Equipment"),
	Consumable  UMETA(DisplayName = "Consumable"),
	Others		UMETA(DisplayName = "Others"),
	Ammo		UMETA(DisplayName = "Ammo"),
};

USTRUCT(BlueprintType)
struct FInventoryConsumable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemImage;

	FInventoryEquipment()
		: ItemID(NAME_None), ItemName(TEXT("")), Quantity(0), ItemImage(nullptr)
	{
	}
};
USTRUCT(BlueprintType)
struct FInventoryOthers
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemImage;

	FInventoryOthers()
		: ItemID(NAME_None), ItemName(TEXT("")), Quantity(0), ItemImage(nullptr)
	{
	}
};

//Add Ammu

USTRUCT(BlueprintType)
struct FInventoryAmmo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* ItemImage;

	FInventoryAmmo()
		: ItemID(NAME_None), ItemName(TEXT("")), Quantity(0), ItemImage(nullptr)
	{
	}
};