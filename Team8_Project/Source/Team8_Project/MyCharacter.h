#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryInterface.h"
#include "MyCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;
class UCapsuleComponent;
class UInventoryComponent;
class ABaseItem;

UCLASS()
class TEAM8_PROJECT_API AMyCharacter : public ACharacter ,public IInventoryInterface
{
	GENERATED_BODY()

// ------------------------------
// Interface Method
// ------------------------------
public:
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
	virtual void SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType)override;

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
	void SetPickableItem(class ABaseItem* OverlappedItem);
protected:
	float NormalSpeed = 600.f;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void ToggleInventory(const FInputActionValue& Value);
	
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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 전투 컴포넌트 초기화
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;





	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float SlowWalkSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float SlowWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsSlowWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bHasWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsAiming = false;

	ABaseItem* PickableItem;

	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void StartJump(const FInputActionValue& value);

	UFUNCTION()
	void StopJump(const FInputActionValue& value);

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& value);

	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	UFUNCTION() //줍기 F
		void StartPickUp(const FInputActionValue& value);

	UFUNCTION()
	void StopPickUp(const FInputActionValue& value);

	UFUNCTION() //앉기 X
		void StartCrouch(const FInputActionValue& value);

	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);

	UFUNCTION() //천천히 걷기 LCtrl
		void StartSlowWalking(const FInputActionValue& value);

	UFUNCTION()
	void StopSlowWalking(const FInputActionValue& value);

	UFUNCTION() //장전
		void StartReload(const FInputActionValue& value);

	UFUNCTION()
	void StopReload(const FInputActionValue& value);

	UFUNCTION() //발사
		void StartFire(const FInputActionValue& value);

	UFUNCTION()
	void StopFire(const FInputActionValue& value);


	UFUNCTION()
	void OnAiming();

	UFUNCTION()
	void ReleaseAiming();

	/*
	전투를 위한 기능들
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPlayerCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* FireRifleAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* AimHipAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName AimSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float PitchInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float YawInput;

public:
	UFUNCTION(BlueprintCallable)
	void PlayFireMontage(bool bAiming);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	// 블루프린트에서 재생


	FORCEINLINE UCameraComponent* GetFollowCamera() const { return Camera; };
};
