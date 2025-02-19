#include "HealingItem.h"
#include "MyCharacter.h" 

AHealingItem::AHealingItem()
{

}
void AHealingItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (AMyCharacter* Player = Cast<AMyCharacter>(Activator))
        {
            UE_LOG(LogTemp, Warning, TEXT("OverLap!"));
        }
        DestroyItem();
    }
}