#include "BaseEnemy.h"

#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();


}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	float RealDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetHP(HP - RealDamage);

	return RealDamage;
}

bool ABaseEnemy::CanAttack()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PlayerController)

		ACharacter* Player = PlayerController->GetCharacter();
	check(Player)

		float Distance = FVector::Distance(Player->GetActorLocation(), GetActorLocation());
	for (int i = 0; i < AttackPatterns.Num(); i++)
		if (Distance <= AttackPatterns[i].AttackRange)
			return true;

	return false;
}

void ABaseEnemy::Attack()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(PlayerController)

		ACharacter* Player = PlayerController->GetCharacter();
	check(Player)

		float Distance = FVector::Distance(Player->GetActorLocation(), GetActorLocation());
	TArray<FAttackPattern> Patterns = AttackPatterns.FilterByPredicate([Distance](const FAttackPattern& Pattern)
		{
			return Distance <= Pattern.AttackRange;
		});

	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			int PatternIndex = FMath::RandRange(0, Patterns.Num() - 1);
			AnimInstance->Montage_Play(AttackPatterns[PatternIndex].Anim);
		}
}

bool ABaseEnemy::IsAttacking()
{
	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			return AnimInstance->IsAnyMontagePlaying();

	return false;
}

void ABaseEnemy::OnDeath()
{
	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			AnimInstance->Montage_Play(DeathMontage);
}

float ABaseEnemy::GetHP() const
{
	return HP;
}

void ABaseEnemy::SetHP(float Value)
{
	HP = Value;

	if (HP <= 0)
	{
		HP = 0;
		OnDeath();
	}
}
