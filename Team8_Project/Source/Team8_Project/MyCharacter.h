#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UCapsuleComponent;
struct  FInputActionValue;

UCLASS()
class TEAM8_PROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();
	void SetPickableItem(class ABaseItem* OverlappedItem);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
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
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float SlowWalk = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSetting")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsCrouching = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementState")
	bool bIsSlowWalking = false;



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

	UFUNCTION() //엎드리기 Z
	void StartProne(const FInputActionValue& value);

	UFUNCTION()
	void StopProne(const FInputActionValue& value);

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


	/*
	전투를 위한 기능들
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPlayerCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combat")
	class UAnimMontage* FireRifleAnimMontage;

	public:
	UFUNCTION()
	void PlayFireMontage(bool bAiming);
};
