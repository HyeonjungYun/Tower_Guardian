#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget.generated.h"

class IInventoryInterface;

UCLASS()
class TEAM8_PROJECT_API UCustomWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void SetOwningActor(AActor* NewOwner);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
