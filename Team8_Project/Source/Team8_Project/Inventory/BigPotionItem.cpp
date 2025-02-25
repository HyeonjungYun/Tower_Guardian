#include "BigPotionItem.h"
#include "../MyCharacter.h" 

ABigPotionItem::ABigPotionItem()
{
    FItemType = "PotionBig";
}

void ABigPotionItem::ActivateItem(AActor* Activator)
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