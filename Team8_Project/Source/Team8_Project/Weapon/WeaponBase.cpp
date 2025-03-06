// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimationAsset.h"
#include "BulletCaseBase.h"
#include "Engine/SkeletalMeshSocket.h" // 탄피배출(BulletCase)
#include "../MyCharacter.h"
#include "../MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCombatComponent.h"

#include "OpticWeaponPartsTable.h"
#include "MuzzleWeaponPartsTable.h"
#include "GripWeaponPartsTable.h"
#include "MagazineWeaponPartsTable.h"
#include "UWeaponPartsUI.h"

#include "Sound/SoundCue.h"
#include "../Inventory/InventoryComponent.h"



AWeaponBase::AWeaponBase() :
	WeaponState(EWeaponState::EWT_Dropped),
	OwnerPlayerCharacter(nullptr),
	OwnerPlayerController(nullptr)
{
	WeaponType = EWeaponType::EWT_None;// 무기 없음초기화

	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	//SetRootComponent(Scene);

	// 스태틱 메시 컴포넌트 생성 및 설정
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(WeaponSkeletalMesh);
	
	// 모두 block pawn은 ignore
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	AreaSphere->SetupAttachment(WeaponSkeletalMesh);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnItemOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnItemEndOverlap);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	InitializeWeaponParts();
	SetWeaponState(WeaponState);

	if (bIsWeaponCanModify)
	{
		EquipWeaponPart(EWeaponPartsType::EWT_Optic, FName("NoOptic"));
		EquipWeaponPart(EWeaponPartsType::EWT_Magazine, FName("NoMuzzle"));
		EquipWeaponPart(EWeaponPartsType::EWT_Grip, FName("NoMagazine"));
		EquipWeaponPart(EWeaponPartsType::EWT_Muzzle, FName("NoGrip"));
	}
}

void AWeaponBase::SetWeaponState(EWeaponState CurWeaponState)
{
	WeaponState = CurWeaponState;
	switch (WeaponState)
	{
	case EWeaponState::EWT_Equipped:
		// ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSkeletalMesh->SetSimulatePhysics(false);
		WeaponSkeletalMesh->SetEnableGravity(false);
		WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		break;
	case EWeaponState::EWT_Dropped:
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponSkeletalMesh->SetSimulatePhysics(true);
		WeaponSkeletalMesh->SetEnableGravity(true);
		WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		WeaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		break;
	}
}

void AWeaponBase::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OwnerPlayerCharacter = Cast<AMyCharacter>(OtherActor);
	if (OwnerPlayerCharacter)
	{
		OwnerPlayerCharacter->SetPickableWeapon(this);
	}
}

void AWeaponBase::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OwnerPlayerCharacter = Cast<AMyCharacter>(OtherActor);
	if (OwnerPlayerCharacter)
	{
		OwnerPlayerCharacter->SetPickableWeapon(nullptr);
	}
}

void AWeaponBase::ActivateItem(AActor* Activator)
{
	// PickUpWidget
	
}

//FName AWeaponBase::GetItemType()
//{
//	return FName();
//}

void AWeaponBase::SpendRound()
{

	OwnerPlayerCharacter = OwnerPlayerCharacter == nullptr ? Cast<AMyCharacter>(GetOwner()) : OwnerPlayerCharacter;
	if (OwnerPlayerCharacter)
	{
		// 탄소모없음
		if (OwnerPlayerCharacter->GetCombatComponent()->bIsInfiniteAmmo)
		{

		}
		else
		{
			CurrentWeaponAmmo = FMath::Clamp(CurrentWeaponAmmo - 1, 0, MaxWeaponAmmo);
			

			if (OwnerPlayerCharacter)
			{
				OwnerPlayerController =
					OwnerPlayerController == nullptr ? Cast<AMyPlayerController>(OwnerPlayerCharacter->Controller) : OwnerPlayerController;
				if (OwnerPlayerController)
				{
					OwnerPlayerController->SetHUDWeaponAmmo(CurrentWeaponAmmo);
				}
			}
		}
	}
}

bool AWeaponBase::IsWeaponMagEmpty()
{
	return CurrentWeaponAmmo <= 0;
}

float AWeaponBase::GetWeaponDamage()
{
	return WeaponDamage;
}

void AWeaponBase::SetWeaponDamage(float NewDamage)
{
	WeaponDamage = NewDamage;
}

void AWeaponBase::OnWeaponEquipped(AMyCharacter* _OwnerPlayerCharacter
	, AMyPlayerController* _OwnerPlayerController)
{
	OwnerPlayerCharacter = _OwnerPlayerCharacter;
	OwnerPlayerController = _OwnerPlayerController;
	if (bIsWeaponCanModify)
	{
		WeaponPartsUI = CreateWidget<UUWeaponPartsUI>(OwnerPlayerController, WeaponPartsUIClass);
		if (WeaponPartsUI)
		{
			WeaponPartsUI->AddToViewport(-50);
			WeaponPartsUI->SetVisibility(ESlateVisibility::Hidden);
			WeaponPartsUI->InitializeAttachmentUI(this);
		}

	}
}

void AWeaponBase::Dropped()
{
	SetWeaponState(EWeaponState::EWT_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponSkeletalMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	OwnerPlayerCharacter = nullptr	;
	OwnerPlayerController = nullptr;
	
	if (WeaponPartsUI)
	{
		WeaponPartsUI->RemoveFromParent();
	}
}

FName AWeaponBase::GetItemType() const
{
	return FName(TEXT("Weapon"));
}

EWeaponType AWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

int32 AWeaponBase::GetCurrrentWeaponAmmo() const
{
	return CurrentWeaponAmmo;
}

void AWeaponBase::SetCurrentWeaponAmmo(int32 _Ammo)
{
	CurrentWeaponAmmo = _Ammo;
}

int32 AWeaponBase::GetMaxWeaponAmmo() const
{
	return MaxWeaponAmmo;
}

void AWeaponBase::SetMaxWeaponAmmo(int32 _Ammo)
{
	MaxWeaponAmmo = _Ammo;
}
void AWeaponBase::ModWeaponPlayFireSound()
{
	if (!WeaponFireSound) return;
	if (!bIsWeaponCanModify) return;

	float FinalVolume = 1.0f; 
	float FinalPitch = 1.0f; 
	if (CurrentWeaponPartsKey.Contains(EWeaponPartsType::EWT_Muzzle))
	{
		FName CurrentMuzzleItemKey = CurrentWeaponPartsKey[EWeaponPartsType::EWT_Muzzle];

		for (int i = 0; i < MuzzleWeaponpartsTableRows.Num(); i++)
		{
			if (MuzzleWeaponpartsTableRows[i]->ItemKey == CurrentMuzzleItemKey)
			{
				FinalVolume *=  MuzzleWeaponpartsTableRows[i]->MuzzleSoundAmount;
				FinalPitch *= MuzzleWeaponpartsTableRows[i]->MuzzleSoundAmount;
			}
		}
	}
	UGameplayStatics::PlaySoundAtLocation(this, WeaponFireSound, GetActorLocation(), FinalVolume, FinalPitch);
}
void AWeaponBase::Fire(const FVector& HitTarget, float CurrentWeaponSpread)
{
	if (bIsWeaponCanModify)
	{
		ModWeaponPlayFireSound();
		if (ModWeaponFireAnimation)
		{
			// 스켈레탈 애니메이션을 반복하지 않고 재생
			WeaponSkeletalMesh->PlayAnimation(ModWeaponFireAnimation, false);
		}
	}
	else
	{
		// 무기 발사
		if (FireAnimation)
		{
			// 스켈레탈 애니메이션을 반복하지 않고 재생
			WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
		}
	}
	

	if (BulletCaseClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket =
			GetWeaponMesh()->GetSocketByName(FName("AmmoEject"));

		if (AmmoEjectSocket)
		{
			// mesh로 부터 탄피배출 위치 소켓의 Transform 받기
			FTransform SocketTransform =
				AmmoEjectSocket->GetSocketTransform(GetWeaponMesh());

			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ABulletCaseBase>(
					BulletCaseClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator());
			}
		}
	}

	SpendRound();
}

float AWeaponBase::GetWeaponZoomFov() const
{
	return ZoomedFOV;
}

float AWeaponBase::GetWeaponZoomInterpSpeed() const
{
	return ZoomInterpSpeed;
}

void AWeaponBase::Reload()
{
	if (OwnerPlayerCharacter)
	{
		OwnerPlayerController =
			OwnerPlayerController == nullptr ? Cast<AMyPlayerController>(OwnerPlayerCharacter->Controller) : OwnerPlayerController;
		if (OwnerPlayerController)
		{
			UInventoryComponent* PlayerInventory
				= OwnerPlayerCharacter->Inventory;
			
			if (PlayerInventory)
			{
				int32 AmmoForReload = MaxWeaponAmmo - CurrentWeaponAmmo;
				int32 ReturnedAmmo = PlayerInventory->ReturnAmmo(AmmoForReload, WeaponType);
				if (ReturnedAmmo != -1)
				{
					CurrentWeaponAmmo += ReturnedAmmo;
					int32 InventoryAmmo
						= PlayerInventory->ReturnCurrentAmmo(WeaponType);
					if (InventoryAmmo == -1)
					{
						InventoryAmmo = 0;
					}
					// HUD 갱신필요
					// 인벤토리 탄 현황
					OwnerPlayerController->SetHUDCarriedAmmo(InventoryAmmo);
					// 무기 탄 현황
					OwnerPlayerController->SetHUDWeaponAmmo(CurrentWeaponAmmo);
					// ammo UI: SetHUDWeaponAmmo(CurrentWeaponAmmo) / SetHUDCarriedAmmo(InventoryAmmo) 
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("탄 없어서 재장전 불가"));
				}
			}			
		}
	}
}

float AWeaponBase::GetTimeToFinishReload()
{
	return TimeToFinishReload;
}

void AWeaponBase::SetTimeToFinishReload(float NewReloadTime)
{
	TimeToFinishReload = NewReloadTime;
}

void AWeaponBase::InitializeWeaponParts()
{
	if (!bIsWeaponCanModify) return;
	// 조준경 테이블에서 조준경 관련 정보를 모두 저장 
	if (!OpticPartsDataTable) return;
	if (!GripPartsDataTable) return;
	if (!MagazinePartsDataTable) return;
	if (!MuzzlePartsDataTable) return;
	OpticPartsDataTable->GetAllRows<FOpticWeaponPartsTable>(TEXT("GetAllRowsFromWeaponPartsTable"), OpticWeaponpartsTableRows);

	// 손잡이 테이블에서 조준경 관련 정보를 모두 저장 
	GripPartsDataTable->GetAllRows<FGripWeaponPartsTable>(TEXT("GetAllRowsFromWeaponPartsTable"), GripWeaponpartsTableRows);

	// 탄창 테이블에서 조준경 관련 정보를 모두 저장 
	MagazinePartsDataTable->GetAllRows<FMagazineWeaponPartsTable>(TEXT("GetAllRowsFromWeaponPartsTable"), MagzineWeaponpartsTableRows);

	// 머즐 테이블에서 조준경 관련 정보를 모두 저장 
	MuzzlePartsDataTable->GetAllRows<FMuzzleWeaponPartsTable>(TEXT("GetAllRowsFromWeaponPartsTable"), MuzzleWeaponpartsTableRows);

	// 2. 각각의 부착물 Actor 생성 및 숨김 처리
	SpawnAndStoreParts(OpticWeaponpartsTableRows, OpticActorContainer, TEXT("ModuleOpticSocket"));
	SpawnAndStoreParts(GripWeaponpartsTableRows, GripActorContainer, TEXT("ModuleForeGripSocket"));
	SpawnAndStoreParts(MagzineWeaponpartsTableRows, MagzineActorContainer, TEXT("ModuleMagazineSocket"));
	SpawnAndStoreParts(MuzzleWeaponpartsTableRows, MuzzleActorContainer, TEXT("ModuleMuzzleSocket"));

}

void AWeaponBase::DebugEnableWeaponParts(FName ItemKey)
{
	// 부착물 타입별 컨테이너 배열
	TArray<TMap<FName, AWeaponpartsActor*>*> PartContainers = {
		&OpticActorContainer,
		&GripActorContainer,
		&MagzineActorContainer,
		&MuzzleActorContainer
	};

	for (TMap<FName, AWeaponpartsActor*>* Container : PartContainers)
	{
		if (!Container) continue;

		// ItemKey가 존재하는 경우
		if (AWeaponpartsActor** FoundPart = Container->Find(ItemKey))
		{
			if (*FoundPart)
			{
				// 기존 부착물 비활성화
				(*FoundPart)->SetActorHiddenInGame(false);
				(*FoundPart)->SetActorEnableCollision(true);

				// 로그 출력
				UE_LOG(LogTemp, Warning, TEXT("Weapon Part %s is now visible"), *ItemKey.ToString());
			}
		}
	}
}

void AWeaponBase::EquipWeaponPart(EWeaponPartsType PartType, FName ItemKey)
{
	if (OwnerPlayerCharacter)
	{
		if (!OwnerPlayerCharacter->GetCombatComponent()->IsThisPartsAvailable(ItemKey))
		{
			UE_LOG(LogTemp, Warning, TEXT("사용 불가능한 파츠"));
			return;
		}
	}

	// 1️⃣ 부착물 타입별 컨테이너 & 현재 장착된 부착물 액터 선택
	TMap<FName, AWeaponpartsActor*>* TargetContainer = nullptr;
	AWeaponpartsActor** EquippedPart = nullptr;

	switch (PartType)
	{
	case EWeaponPartsType::EWT_Optic:
		TargetContainer = &OpticActorContainer;
		EquippedPart = &EquippedOpticPart;
		break;
	case EWeaponPartsType::EWT_Grip:
		TargetContainer = &GripActorContainer;
		EquippedPart = &EquippedGripPart;
		break;
	case EWeaponPartsType::EWT_Magazine:
		TargetContainer = &MagzineActorContainer;
		EquippedPart = &EquippedMagazinePart;
		break;
	case EWeaponPartsType::EWT_Muzzle:
		TargetContainer = &MuzzleActorContainer;
		EquippedPart = &EquippedMuzzlePart;
		break;
	default:
		return;
	}

	if (!TargetContainer || !EquippedPart) return;

	// 2️⃣ 기존 부착물 해제 (투명화 & 충돌 제거)
	if (*EquippedPart)
	{
		(*EquippedPart)->SetActorHiddenInGame(true);
		//(*EquippedPart)->SetActorEnableCollision(false);
		UE_LOG(LogTemp, Warning, TEXT("Unequipped %s"), *CurrentWeaponPartsKey[PartType].ToString());
	}

	// 3️⃣ 새로운 부착물 장착 (투명 해제 & 충돌 활성화)
	if (AWeaponpartsActor** NewPart = TargetContainer->Find(ItemKey))
	{
		if (*NewPart)
		{
			(*NewPart)->SetActorHiddenInGame(false);
			//(*NewPart)->SetActorEnableCollision(true);

			// 4️⃣ 현재 장착된 부착물 정보 업데이트
			*EquippedPart = *NewPart;
			CurrentWeaponPartsKey.Add(PartType, ItemKey);

			UE_LOG(LogTemp, Warning, TEXT("Equipped %s"), *ItemKey.ToString());
		}
	}
	
}


