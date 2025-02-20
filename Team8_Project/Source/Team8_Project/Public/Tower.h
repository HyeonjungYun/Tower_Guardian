#pragma once

#include "CoreMinimal.h"
#include "Team8_Project/Damageable.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

UCLASS()
class TEAM8_PROJECT_API ATower : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	ATower();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerHP")
	float TowerHP;

	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	float GetHP() const override;
	void SetHP(float value) override;

};
