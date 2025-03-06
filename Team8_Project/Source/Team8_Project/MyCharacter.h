#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryInterface.h"
#include "Damageable.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;
class UCapsuleComponent;
class UInventoryComponent;
class ABaseItem;
enum class EWeaponType : uint8;

UENUM(BlueprintType)
enum class EPlayerStateType : uint8
{
	EWT_Normal UMETA(DisplayName = "Normal"),
	EWT_Punch UMETA(DisplayName = "Punch"),
	EWT_Fire UMETA(DisplayName = "Fire"),
	EWT_Reload UMETA(DisplayName = "Reload"),
	EWT_ChangeWeapon UMETA(DisplayName = "ChangeWeapon"),
	EWT_Damaged UMETA(DisplayName = "Damaged"),
	EWT_Dead UMETA(DisplayName = "Dead")

};

UCLASS()
class TEAM8_PROJECT_API AMyCharacter : public ACharacter ,public IInventoryInterface, public IDamageable
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
	virtual void SortAmmoItems(bool bIsAscending) override;
	// Get Method
	virtual int32 GetGold() const override;
	virtual const TArray<FInventoryConsumable>& GetConsumableItems() const override;
	virtual const TArray<FInventoryEquipment>& GetEquipmentItems() const override;
	virtual const TArray<FInventoryOthers>& GetOthersItems() const override;
	virtual const TArray<FInventoryAmmo>& GetAmmoItems() const override;
	virtual float GetHP() const override;
	// Set Method
	virtual bool AddItem(const FName& ItemKey, int32 Quantity, EItemType ItemType) override;
	virtual bool RemoveItem(const FName& ItemKey, int32 Quantity) override;
	virtual bool UseItem(int32 SlotIndex, EItemType ItemType) override;
	virtual void SetGold(int32 NewGold) override;
	virtual void SwapItem(int32 PrevIndex, int32 CurrentIndex, EItemType PrevSlotType, EItemType CurrentSlotType)override;
	virtual void SetHP(float Value) override;

	//Search Method
	virtual int32 SearchItemByNameAndType(const FName& ItemKey, const EItemType& ItemType) const override;
	virtual	int32 SearchItemByName(const FName& ItemKey) const override;
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

	UFUNCTION(BlueprintCallable, Category = "inventory")
	void ToggleInventory(const FInputActionValue& Value);

	void SetPickableItem(class ABaseItem* OverlappedItem);

	void SetPickableWeapon(class AWeaponBase* OverlappedWeapon);

	AWeaponBase* PickableWeapon = nullptr;
protected:
	float NormalSpeed = 600.f;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float SlowWalkSpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float SlowWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float RotationSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	FRotator NewRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	EPlayerStateType PlayerStates = EPlayerStateType::EWT_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	EWeaponType PreWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsSlowWalking = false;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	int32 ShotgunReloadCount = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> PickupSound;
	
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

	UFUNCTION() //이동 시 캐릭터 보간 회전, turn in place 를 위한 
	void CalculateRotation(float DeltaTime);

	UFUNCTION()
	int32 GetShotgunReloadCount();

	UFUNCTION()
	void OnAiming();

	UFUNCTION()
	void ReleaseAiming();

	UFUNCTION()
	void OnWeaponModifyingInput();
	bool bIsWMUIOpened = false;

	UFUNCTION()
	void OnWeaponModArrowUp();
	UFUNCTION()
	void OnWeaponModArrowDown();
	UFUNCTION()
	void OnWeaponModArrowLeft();
	UFUNCTION()
	void OnWeaponModArrowRight();
	/*
	전투를 위한 기능들
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPlayerCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* FireRifleAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName AimSectionName;

	//펀치
	UPROPERTY()
	TArray<class UAnimMontage*> PunchMontages;

	int32 FunchAnimIndex = 0;
	int32 CurrentFunchAnimIndex = 0;
	int32 FunchAnimMaxIndex = 0;
	FTimerHandle FunchComboTimerHandle;
	void FunchCombo(int32 AnimIndex);
	void ResetFunchCombo();
	void StopPunch();//펀치중 wasd 움직임이 발생하면 멈춤


	// 카메라가 벽에 겹쳐졌을 때 플레이어 모델링이 가리는 문제 해결하기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraThreshold = 200.f;
	void HideCameraIfCharacterClose();
public:
	UFUNCTION(BlueprintCallable)
	void PlayFireMontage(bool bAiming);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	// 블루프린트에서 재생
	UPlayerCombatComponent* GetCombatComponent();

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return Camera; };
	/*
	// HUD 접근을 위한
	// 멤버 변수 및 함수
	*/
public:
	class AMyPlayerController* MyPlayerController;
	 

	/*
		사망 관련
	*/
public:
		UFUNCTION()
		virtual float TakeDamage
		(
			float DamageAmount,
			struct FDamageEvent const& DamageEvent,
			class AController* EventInstigator,
			AActor* DamageCauser
		) override;

		UFUNCTION(BlueprintCallable)
		virtual void OnDeath();
	//UFUNCTION(BlueprintCallable)
	//float GetHP();
	//UFUNCTION(BlueprintCallable)
	//void SetHP(float setHp);
	float GetMaxHP();

private:
		FTimerHandle SpeedBoostTimerHandle;
		float BaseWalkSpeed = 400.f;
public:
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void ApplySpeedBoost(float Percent, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Speed")
	float GetPlayerSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsInventoryVisible = false;
	//
};
