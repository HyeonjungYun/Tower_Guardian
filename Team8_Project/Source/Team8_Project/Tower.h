#pragma once

#include "CoreMinimal.h"
#include "Team8_Project/Damageable.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class TEAM8_PROJECT_API ATower : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	ATower();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HPBar")
	UWidgetComponent* TowerHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerHP")
	float TowerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerHP")
	float TowerMaxHP;

	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	float GetHP() const override;
	void SetHP(float value) override;

};
