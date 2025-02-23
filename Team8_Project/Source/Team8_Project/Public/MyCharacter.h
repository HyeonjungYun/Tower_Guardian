// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryInterface.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

class UInventoryComponent;
class ABaseItem;


UCLASS()
class TEAM8_PROJECT_API AMyCharacter : public ACharacter ,public IInventoryInterface
{
	GENERATED_BODY()

public:
// ------------------------------
// Interface Method
// ------------------------------
	// Sort Method
	virtual void SortEquipmentItems(bool bIsAscending) override;
	virtual void SortConsumableItems(bool bIsAscending) override;
	virtual void SortOthersItems(bool bIsAscending) override;
	// Get Method
	virtual int32 GetGold() const override;
	virtual const TArray<FInventoryConsumable>& GetConsumableItems() const override;
	virtual const TArray<FInventoryEquipment>& GetEquipmentItems() const override;
	virtual const TArray<FInventoryOthers>& GetOthersItems() const override;
	// Set Method
	virtual bool AddItem(const FName& ItemKey, int32 Quantity) override;
	virtual bool RemoveItem(const FName& ItemKey, int32 Quantity) override;
	virtual bool UseItem(int32 SlotIndex, EItemType ItemType) override;
	virtual void SetGold(int32 NewGold) override;


public:
	AMyCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "inventory")
	UInventoryComponent* Inventory;

protected:
	float NormalSpeed = 600.f;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void PickUp(const FInputActionValue& Value);


	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void OnPickupItem();

	UPROPERTY()
	ABaseItem* OverlappingItem;

};
