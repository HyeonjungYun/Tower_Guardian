// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class TEAM8_PROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PickUpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryToggleAction = nullptr;

protected:
	virtual void BeginPlay() override;

};
