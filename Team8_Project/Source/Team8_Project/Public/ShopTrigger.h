#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShopTrigger.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class TEAM8_PROJECT_API AShopTrigger : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:	
	AShopTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShopTrigger")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "ShopUI")
	TSubclassOf<UUserWidget> ShopWidgetClass;

	UPROPERTY()
	UUserWidget* ShopWidgetInstance;

	bool bIsPlayerInRange;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex
	);

	void SetupInputComponent();
	void OpenShop();

private:
	UInputComponent* InputComponent;
};
