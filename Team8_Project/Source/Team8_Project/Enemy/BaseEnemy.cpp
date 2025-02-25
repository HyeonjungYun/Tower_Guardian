#include "BaseEnemy.h"

#include "EnemyAIController.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	AI_Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_Perception"));

	// AI 시야 구성 설정
	AI_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	AI_Sight->SightRadius = 1000.f;
	AI_Sight->LoseSightRadius = 1500.f;
	AI_Sight->PeripheralVisionAngleDegrees = 90.f;
	AI_Sight->AutoSuccessRangeFromLastSeenLocation = -1.f;

	// Detect only specific actors 
	AI_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AI_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AI_Sight->DetectionByAffiliation.bDetectFriendlies = true;

	AI_Perception->ConfigureSense(*AI_Sight);
	AI_Perception->SetDominantSense(AI_Sight->GetSenseImplementation());
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	AI_Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemy::OnTargetPerceptionUpdated);
}

void ABaseEnemy::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ACharacter* Player = Cast<ACharacter>(Actor);
	if (!Player) return;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard) return;
	
	if (Stimulus.WasSuccessfullySensed())
	{
		BlackBoard->SetValueAsBool("DetectedPlayer", true);
		BlackBoard->SetValueAsObject("TargetPlayer", Player);
	}
	else
	{
		BlackBoard->SetValueAsBool("DetectedPlayer", false);
		BlackBoard->SetValueAsObject("TargetPlayer", nullptr);
	}
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
	return CanAttackToType(nullptr);
}

bool ABaseEnemy::CanAttackToType(TSubclassOf<AActor> AttackType)
{
	TArray<FOverlapResult> Array;
	return CanAttackToType(AttackType, Array);
}

bool ABaseEnemy::CanAttackToType(TSubclassOf<AActor> AttackType, TArray<FOverlapResult>& OutOverlapResults)
{
	//공격 중 제일 사정거리가 긴 공격
	float MaxAttackRange = GetMaxAttackRange();
	bool bOverlap = GetWorld()->OverlapMultiByObjectType(OutOverlapResults,
	                                                     GetActorLocation(), GetActorQuat(),
	                                                     FCollisionObjectQueryParams::DefaultObjectQueryParam,
	                                                     FCollisionShape::MakeSphere(MaxAttackRange));

	DrawDebugSphere(GetWorld(), GetActorLocation(), MaxAttackRange, 30, FColor::Red);
	if (bOverlap)
	{
		for (auto& Element : OutOverlapResults)
		{
			if (Element.GetActor() != this && Element.GetActor()->Implements<UDamageable>() &&
				(AttackType == nullptr || Element.GetActor()->IsA(AttackType.Get())))
				return true;
		}
	}

	return false;
}

void ABaseEnemy::Attack()
{
	TArray<FOverlapResult> OverlapResults;
	if (!CanAttackToType(nullptr, OverlapResults))
		return;

	//가장 먼 공격 가능한 오브젝트와의 거리
	float NearRange = MAX_FLT;
	for (auto& Element : OverlapResults)
	{
		FVector ClosetLocation;
		Element.GetComponent()->GetClosestPointOnCollision(GetActorLocation(), ClosetLocation);

		// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, Element.GetComponent()->GetName());
		// DrawDebugSphere(GetWorld(), ClosetLocation, 30, 20, FColor::Green);

		float Distance = FVector::Distance(ClosetLocation, GetActorLocation());
		if (Distance < NearRange)
			NearRange = Distance;
	}

	//뭔가 잘못되서 OverlapResults안에서 Distance 체크를 제대로 못함
	if (NearRange == MAX_FLT)
	{
		UE_LOG(LogTemp, Warning, TEXT("공격할 수 없습니다."));
		return;
	}

	//공격 패턴이 없을 경우 실행하지 않음
	if (AttackPatterns.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("공격 패턴이 없습니다."));
		return;
	}

	TArray<FAttackPattern> Patterns = AttackPatterns.FilterByPredicate([NearRange](const FAttackPattern& Pattern)
	{
		// - 10.f는 근삿값, 살짝 거리 있다고 실행 안되는걸 방지하기 위함
		return NearRange - 10.f <= Pattern.AttackRange;
	});

	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("NearRange: %f"), NearRange));
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("MaxAttackRange: %f"), MaxAttackRange));

	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			//Skeletal Mesh 같은거에 Collider가 또 있는데, Overlap 처리되면 애니메이션이 검출되지 않을 수 있음.
			//만약 아무런 Pattern이 검출되지 않았다면 제일 짧은걸 사용하도록 함
			FAttackPattern* PlayPattern = nullptr;
			if (Patterns.Num() != 0)
			{
				int PatternIndex = FMath::RandRange(0, Patterns.Num() - 1);
				PlayPattern = &AttackPatterns[PatternIndex];
			}
			else
			{
				//닿긴 닿았는데 외곽에 닿아서 다른 충돌로 인해 제대로 안닿았다는 소리니까 제일 긴 패턴을 검출해서 실행
				for (FAttackPattern& Pattern : AttackPatterns)
					if (PlayPattern == nullptr ||
						(Pattern.AttackRange > PlayPattern->AttackRange))
						PlayPattern = &Pattern;
			}

			AnimInstance->Montage_Play(PlayPattern->Anim);
		}
}

bool ABaseEnemy::IsAttacking()
{
	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			return AnimInstance->IsAnyMontagePlaying();

	return false;
}

void ABaseEnemy::SetPatrolPath(APatrolPath* Value)
{
	PatrolPath = Value;
}

FVector ABaseEnemy::GetPatrolLocation() const
{
	if (PatrolPath)
		return PatrolPath->GetWaypoint(PatrolIndex);

	//없다면 본인 위치
	UE_LOG(LogTemp, Warning, TEXT("PatrolPath Not Setting!"));
	return GetActorLocation();
}

void ABaseEnemy::SetPatrolLocationToNext()
{
	if (PatrolIndex < PatrolPath->Num() - 1)
		PatrolIndex++;
}

void ABaseEnemy::OnDeath()
{
	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			AnimInstance->Montage_Play(DeathMontage);
}

float ABaseEnemy::GetMaxAttackRange() const
{
	float result = -1;
	for (auto& Pattern : AttackPatterns)
		if (Pattern.AttackRange > result)
			result = Pattern.AttackRange;

	return result;
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
