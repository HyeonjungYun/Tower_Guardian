#include "PotionItem.h"
#include "../MyCharacter.h" 

APotionItem::APotionItem()
{
    FItemType = "PotionSmall";
    SlotType = EItemType::Consumable;
}

void APotionItem::ActivateItem(AActor* Activator)
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
