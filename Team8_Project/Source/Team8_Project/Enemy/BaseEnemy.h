#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Team8_Project/Damageable.h"
#include "BaseEnemy.generated.h"

class ABaseItem;
class ASpawnVolume;
struct FAIStimulus;
class UBehaviorTree;
class UAISenseConfig_Sight;
class UAIPerceptionComponent;
class UFloatingPawnMovement;
class APatrolPath;
class ABaseEnemy;

DECLARE_MULTICAST_DELEGATE_OneParam(FEnemyDeathDelegate, ABaseEnemy*)

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
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	bool CanAttack();

	UFUNCTION(BlueprintCallable)
	bool CanAttackWithType(TSubclassOf<AActor> AttackType = nullptr);
	bool CanAttackWithType(TArray<FOverlapResult>& OutOverlapResults, TSubclassOf<AActor> AttackType = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void Attack(TSubclassOf<AActor> AttackType = nullptr, bool Shortest = false);

	UFUNCTION(BlueprintCallable)
	bool IsAttacking();
	
	UFUNCTION(BlueprintCallable)
	void SetWaypointLocationToNext();

	UFUNCTION(BlueprintCallable)
	FVector GetWaypointLocation() const;

	void SetSpawnVolume(ASpawnVolume* Value);
	
	virtual float GetHP() const override;
	virtual void SetHP(float Value) override;

protected:
	virtual void BeginPlay() override;
	
	virtual void Death();
	
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintNativeEvent)
	void OnDeathMontageEnd();
	void OnDeathMontageEnd_Implementation();

private:
	float GetMaxAttackRange() const;
	void RemoveUnattackableActor(TArray<FOverlapResult>& OutOverlapResults, TSubclassOf<AActor> AttackType);
	int GetWeightRandomIndex(int ArraySize) const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFloatingPawnMovement> PawnMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UAIPerceptionComponent> AI_Perception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UAISenseConfig_Sight> AI_Sight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float HP;

	FEnemyDeathDelegate OnDeath;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	TArray<FAttackPattern> AttackPatterns;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitReactionMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HitReactionCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotationMul = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropGold = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ABaseItem>> DropItemClasses;
	
	TObjectPtr<ASpawnVolume> SpawnVolume;
	int32 WaypointIndex;
	
private:
	int32 CurrentHitReactionCount = 0;
};

