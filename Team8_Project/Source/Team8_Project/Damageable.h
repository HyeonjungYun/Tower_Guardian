#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAM8_PROJECT_API IDamageable
{
	GENERATED_BODY()

public:
	virtual float GetHP() const = 0;
	virtual void SetHP(float Value) = 0;
	
};
