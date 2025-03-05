#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyCharacter.h"
// Sets default values
ABaseItem::ABaseItem()
{
    PrimaryActorTick.bCanEverTick = false;
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(StaticMesh);
    
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
    StaticMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
    StaticMesh->SetSimulatePhysics(true);
   

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    Collision->SetupAttachment(StaticMesh);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}


void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        // ActivateItem(OtherActor);
        // 아이템 사용 (획득) 로직 호출
        AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherActor);
        if (PlayerCharacter)
        {
            PlayerCharacter->SetPickableItem(this);
        }
    }
}

void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
       
        AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(OtherActor);
        if (PlayerCharacter)
        {// 범위에서 벗어난 경우 nullptr 초기화
            PlayerCharacter->SetPickableItem(nullptr);
        }
    }
}


void ABaseItem::ActivateItem(AActor* Activator)
{
    UParticleSystemComponent* Particle = nullptr;

    if (PickupParticle)
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            PickupParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );
    }

    if (Particle)
    {
        FTimerHandle DestroyParticleTimerHandle;
        TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;

        GetWorld()->GetTimerManager().SetTimer(
            DestroyParticleTimerHandle,
            [WeakParticle]()
            {
                WeakParticle->DestroyComponent();
            },
            0.1f,
            false
        );
    }
}



FName ABaseItem::GetItemType() const
{
    return FItemType;
}

void ABaseItem::DestroyItem()
{
    Destroy();
}

EItemType ABaseItem::GetSlotType() const
{
    return SlotType;
}

int32 ABaseItem::GetQuantity()
{
    return Quantity;
}

