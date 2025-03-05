#include "BaseEnemy.h"

#include "BrainComponent.h"
#include "EnemyAIController.h"
#include "Team8_Project/Spawn/SpawnVolume.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ShapeComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Team8_Project/BaseItem.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/WorldSpawnUISubSystem.h"
#include "Team8_Project/Inventory/InventorySubsystem.h"


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

	HP = MaxHP;

	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController()))
	{
		if (BehaviorTree)
			EnemyController->RunBehaviorTree(BehaviorTree);
		else
			UE_LOG(LogTemp, Warning, TEXT("BehaviorTree가 없습니다"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Controller를 변환하지 못했습니다."));

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
	if (GetHP() <= 0)
		return 0.f;

	float RealDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	FVector DamageTextLoc = GetActorLocation();
	DamageTextLoc.Z += GetComponentByClass<UShapeComponent>()->Bounds.BoxExtent.Z * 2;

	SetHP(HP - RealDamage);
	GetGameInstance()->GetSubsystem<UWorldSpawnUISubSystem>()->SpawnDamageText(GetWorld(), RealDamage, DamageTextLoc);

	if (HP > 0)
		if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
				if (AnimInstance->IsAnyMontagePlaying() == false && CurrentHitReactionCount <= 0)
				{
					CurrentHitReactionCount = HitReactionCount;
					AnimInstance->Montage_Play(HitReactionMontage);
				}
				else
					CurrentHitReactionCount--;

	return RealDamage;
}

void ABaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetHP() <= 0)
		return;

	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			if (!AnimInstance->IsAnyMontagePlaying() && PawnMovement->Velocity.SquaredLength() > 0.f)
			{
				FRotator TargetRot = PawnMovement->Velocity.Rotation();
				TargetRot.Roll = 0;
				TargetRot.Pitch = 0;

				FRotator ViewRot = UKismetMathLibrary::RLerp(GetActorRotation(), TargetRot,
				                                             DeltaSeconds * RotationMul, true);

				SetActorRotation(ViewRot);
			}
}

bool ABaseEnemy::CanAttack()
{
	return CanAttackWithType();
}

bool ABaseEnemy::CanAttackWithType(TSubclassOf<AActor> AttackType)
{
	TArray<FOverlapResult> Array;
	return CanAttackWithType(Array, AttackType);
}

bool ABaseEnemy::CanAttackWithType(TArray<FOverlapResult>& OutOverlapResults, TSubclassOf<AActor> AttackType)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//공격 중 제일 사정거리가 긴 공격
	float MaxAttackRange = GetMaxAttackRange();
	bool bOverlap = GetWorld()->OverlapMultiByObjectType(OutOverlapResults,
	                                                     GetActorLocation(), GetActorQuat(),
	                                                     FCollisionObjectQueryParams::DefaultObjectQueryParam,
	                                                     FCollisionShape::MakeSphere(MaxAttackRange),
	                                                     Params);

	DrawDebugSphere(GetWorld(), GetActorLocation(), MaxAttackRange, 30, FColor::Red);

	if (bOverlap)
	{
		RemoveUnattackableActor(OutOverlapResults, AttackType);
		return OutOverlapResults.Num() > 0;
	}

	return false;
}

void ABaseEnemy::Attack(TSubclassOf<AActor> AttackType, bool Shortest)
{
	TArray<FOverlapResult> OverlapResults;
	if (!CanAttackWithType(OverlapResults, AttackType))
		return;

	AActor* NearActor = nullptr;
	//가장 가까운 공격 가능한 오브젝트와의 거리
	float NearRange = MAX_FLT;
	for (auto& Element : OverlapResults)
	{
		FVector ClosetLocation;
		Element.GetComponent()->GetClosestPointOnCollision(GetActorLocation(), ClosetLocation);

		// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, Element.GetActor()->GetName());
		// DrawDebugSphere(GetWorld(), ClosetLocation, 30, 20, FColor::Green, false, 1);

		float Distance = FVector::Distance(ClosetLocation, GetActorLocation());
		if (Distance < NearRange)
		{
			NearRange = Distance;
			NearActor = Element.GetActor();
		}
	}

	//뭔가 잘못되서 OverlapResults안에서 Distance 체크를 제대로 못함
	if (NearRange == MAX_FLT || NearActor == nullptr)
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

	//공격 대상으로 회전
	FVector ViewVec = NearActor->GetActorLocation() - GetActorLocation();
	FRotator ViewRot = ViewVec.Rotation();
	ViewRot.Roll = 0;
	ViewRot.Pitch = 0;

	SetActorRotation(ViewRot);

	TArray<FAttackPattern> Patterns = AttackPatterns.FilterByPredicate([NearRange](const FAttackPattern& Pattern)
	{
		// - 10.f는 근삿값, 살짝 거리 있다고 실행 안되는걸 방지하기 위함
		return NearRange - 10.f <= Pattern.AttackRange;
	});

	// for (auto& Pattern : Patterns)
	// 	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, Pattern.Anim->GetName());

	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("NearRange: %f"), NearRange));
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("MaxAttackRange: %f"), MaxAttackRange));

	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			FAttackPattern* PlayPattern = nullptr;
			if (Shortest)
			{
				//타워 공격용 제일 짧은 공격을 실행
				Patterns.Sort([](const FAttackPattern& Ele, const FAttackPattern& Ele2)
				{
					return Ele.AttackRange < Ele2.AttackRange;
				});

				PlayPattern = &Patterns[0];
			}
			else if (Patterns.Num() > 1)
			{
				//사거리 짧은 공격이 더 많이 나올 수 있음
				//사거리 순서로 오름차순 정렬
				Patterns.Sort([](const FAttackPattern& Ele, const FAttackPattern& Ele2)
				{
					return Ele.AttackRange < Ele2.AttackRange;
				});

				int Index = GetWeightRandomIndex(Patterns.Num());
				// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Index ") + FString::FromInt(Index));

				PlayPattern = &Patterns[Index];
			}
			else if (Patterns.Num() == 1)
			{
				PlayPattern = &Patterns[0];
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

void ABaseEnemy::SetSpawnVolume(ASpawnVolume* Value)
{
	SpawnVolume = Value;
}

FVector ABaseEnemy::GetWaypointLocation() const
{
	if (SpawnVolume)
		return SpawnVolume->GetWaypoint(WaypointIndex);

	//없다면 본인 위치
	UE_LOG(LogTemp, Warning, TEXT("PatrolPath Not Setting!"));
	return GetActorLocation();
}

void ABaseEnemy::SetWaypointLocationToNext()
{
	if (!SpawnVolume)
	{
		//없다면 본인 위치
		UE_LOG(LogTemp, Warning, TEXT("SpawnVolume Not Setting!"));
		return;
	}

	if (WaypointIndex < SpawnVolume->WaypointCount() - 1)
		WaypointIndex++;
}

void ABaseEnemy::Death()
{
	//골드 넣어줌
	UInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	Inventory->SetGold(Inventory->GetGold() + DropGold);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Death"));

	//아이템 떨굼
	FVector SpawnLo = GetActorLocation();
	FRotator SpawnRot = GetActorRotation();
	for (auto& DropItemClass : DropItemClasses)
		GetWorld()->SpawnActor(DropItemClass.Get(), &SpawnLo, &SpawnRot);

	//사망 모션 플레이
	if (USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>())
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage, 1);

			//EndDelegate가 몽타주 설정 때문에 실행을 안해서 Timer로 만듦
			FTimerHandle DeathTimer;
			GetWorldTimerManager().SetTimer(DeathTimer, this, &ABaseEnemy::OnDeathMontageEnd, DeathMontage->GetPlayLength(),
			                                false);
		}
	}

	if (AAIController* AIController = GetController<AAIController>())
	{
		AIController->StopMovement();
		AIController->GetBrainComponent()->StopLogic(TEXT("Death Monster"));
		AIController->UnPossess();
	}

	AI_Perception->SetSenseEnabled(UAISenseConfig_Sight::StaticClass(), false);

	OnDeath.Broadcast(this);
}

float ABaseEnemy::GetMaxAttackRange() const
{
	float result = -1;
	for (auto& Pattern : AttackPatterns)
		if (Pattern.AttackRange > result)
			result = Pattern.AttackRange;

	return result;
}

void ABaseEnemy::RemoveUnattackableActor(TArray<FOverlapResult>& OutOverlapResults, TSubclassOf<AActor> AttackType)
{
	for (int i = 0; i < OutOverlapResults.Num(); i++)
	{
		FOverlapResult& Element = OutOverlapResults[i];

		if ((Element.GetActor() != this && Element.GetActor()->Implements<UDamageable>() &&
				(AttackType == nullptr || Element.GetActor()->IsA(AttackType.Get())) &&
				!Element.GetActor()->IsA<ABaseEnemy>())
			== false)
		{
			OutOverlapResults.RemoveAt(i);
			i--;
		}
	}
}

int ABaseEnemy::GetWeightRandomIndex(int ArraySize) const
{
	TArray<float> Weights;
	float TotalWeight = 0.0f;
	int LastIndex = ArraySize - 1;

	// 가중치 부여 (작은 인덱스일수록 확률이 높음)
	for (int i = 0; i <= LastIndex; i++)
	{
		float Weight = (LastIndex - i) + 1; // 예: 5, 4, 3, 2, 1 식으로 줄어듦
		Weights.Add(Weight * Weight); // 제곱해서 낮은 숫자가 더 잘 걸리게 가중치를 설정
		TotalWeight += Weight;
	}

	// 랜덤 값 생성
	float RandomValue = FMath::FRand() * TotalWeight;
	float CumulativeWeight = 0.0f;

	// 누적 가중치를 사용해 랜덤 인덱스 선택
	for (int i = 0; i <= LastIndex; i++)
	{
		CumulativeWeight += Weights[i];
		if (RandomValue <= CumulativeWeight)
		{
			return i;
		}
	}

	// 예외 처리
	return LastIndex;
}

void ABaseEnemy::OnDeathMontageEnd_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Death_Default"));
	Destroy();
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
		Death();
	}
}
