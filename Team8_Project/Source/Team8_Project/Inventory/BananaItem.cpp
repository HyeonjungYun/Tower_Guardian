#include "BananaItem.h"
#include "../MyCharacter.h" 

ABananaItem::ABananaItem()
{
    FItemType = "Banana";
    SlotType = EItemType::Ammo;
}

void ABananaItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (AMyCharacter* Player = Cast<AMyCharacter>(Activator))
        {
            UE_LOG(LogTemp, Warning, TEXT("OverLap!"));
            UE_LOG(LogTemp, Warning, TEXT("%s!"), *FItemType.ToString());
        }
        /*DestroyItem();*/
    }
}