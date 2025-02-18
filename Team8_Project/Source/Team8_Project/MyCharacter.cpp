#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h" //뷰포트에 로그를 출력 위함

AMyCharacter::AMyCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	Capsule = GetCapsuleComponent(); //기본 캡슐 컴포넌트만 반환

	//Capsule컴포넌트 -> Overlap에 이벤트 바인딩

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SprintSpeed = WalkSpeed * SprintSpeedMultiplier;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//액션 바인딩
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartJump
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopJump
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSprint
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSprint
				);
			}

			//
			if (PlayerController->PickUpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PickUpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartPickUp
				);
			}

			if (PlayerController->PickUpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->PickUpAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopPickUp
				);
			}
			//
			if (PlayerController->ProneAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ProneAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartProne
				);
			}

			if (PlayerController->ProneAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ProneAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopProne
				);
			}
			//
			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartCrouch
				);
			}

			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopCrouch
				);
			}
			//
			if (PlayerController->SlowWalkingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SlowWalkingAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSlowWalking
				);
			}

			if (PlayerController->SlowWalkingAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SlowWalkingAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSlowWalking
				);
			}
			//
			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartReload
				);
			}

			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopReload
				);
			}
			//
			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartFire
				);
			}

			if (PlayerController->FireAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopFire
				);
			}
			
		}
	}

}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddControllerYawInput(LookInput.X * MouseSensitivity);
	}

	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		AddControllerPitchInput(LookInput.Y * MouseSensitivity);
	}
}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void AMyCharacter::StartPickUp(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartPickUp!"));
			}
		}
	}
}

void AMyCharacter::StopPickUp(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopPickUp!"));
			}
		}
	}
}

void AMyCharacter::StartProne(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartProne!"));
			}
		}
	}
}

void AMyCharacter::StopProne(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopProne!"));
			}
		}
	}
}

void AMyCharacter::StartCrouch(const FInputActionValue& value)
{
	if (!bIsCrouching)
	{
		bIsCrouching = true;

		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartCrouch!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = SlowWalk;
		Crouch();
	}
	
}

void AMyCharacter::StopCrouch(const FInputActionValue& value)
{
	if (bIsCrouching)
	{
		bIsCrouching = false;

		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopCrouch!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		UnCrouch();
	}
}

void AMyCharacter::StartSlowWalking(const FInputActionValue& value)
{
	if (!bIsSlowWalking)
	{
		bIsSlowWalking = true;

		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartSlowWalking!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = SlowWalk;
	}
}

void AMyCharacter::StopSlowWalking(const FInputActionValue& value)
{
	if (bIsSlowWalking)
	{
		bIsSlowWalking = false;

		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopSlowWalking!"));
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyCharacter::StartReload(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartReload!"));
			}
		}
	}
}

void AMyCharacter::StopReload(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopReload!"));
			}
		}
	}
}

void AMyCharacter::StartFire(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StartFire!"));
			}
		}
	}
}

void AMyCharacter::StopFire(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			if (GEngine) //for debug
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("StopFire!"));
			}
		}
	}
}

