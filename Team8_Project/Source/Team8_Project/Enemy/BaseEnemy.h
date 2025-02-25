#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Team8_Project/Damageable.h"
#include "BaseEnemy.generated.h"

class UAISenseConfig_Sight;
class UAIPerceptionComponent;
class UFloatingPawnMovement;
class APatrolPath;

USTRUCT(BlueprintType)
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
	bool CanAttackToType(TSubclassOf<AActor> AttackType);
	bool CanAttackToType(TSubclassOf<AActor> AttackType, TArray<FOverlapResult>& OutOverlapResults);
	
	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	bool IsAttacking();
	
	UFUNCTION(BlueprintCallable)
	void SetPatrolLocationToNext();

	UFUNCTION(BlueprintCallable)
	FVector GetPatrolLocation() const;

	//임시로 달아줌. 스포너에서 달아주는 것으로 변경 후 삭제
	UFUNCTION(BlueprintCallable)
	void SetPatrolPath(APatrolPath* Value);
	
	virtual float GetHP() const override;
	virtual void SetHP(float Value) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	virtual void OnDeath();

private:
	float GetMaxAttackRange() const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFloatingPawnMovement> PawnMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UAIPerceptionComponent> AI_Perception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UAISenseConfig_Sight> AI_Sight;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	TArray<FAttackPattern> AttackPatterns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;
	
	TObjectPtr<APatrolPath> PatrolPath;
	int32 PatrolIndex;
	
private:
	float HP;
};
