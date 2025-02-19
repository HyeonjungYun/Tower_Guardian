#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Team8_Project/Damageable.h"
#include "BaseEnemy.generated.h"

class APatrolPath;

USTRUCT(Atomic, BlueprintType)
struct FAttackPattern
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;
};

UCLASS()
class TEAM8_PROJECT_API ABaseEnemy : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	bool CanAttack();
	
	UFUNCTION(BlueprintCallable)
	void Attack();
	
	UFUNCTION(BlueprintCallable)
	bool IsAttacking();

	virtual float GetHP() const override;
	virtual void SetHP(float Value) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnDeath();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Attack")
	TObjectPtr<APatrolPath> PatrolPath;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	TArray<FAttackPattern> AttackPatterns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

private:
	float HP;
};
