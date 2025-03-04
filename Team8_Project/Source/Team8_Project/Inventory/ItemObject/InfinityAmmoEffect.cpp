// Fill out your copyright notice in the Description page of Project Settings.


#include "InfinityAmmoEffect.h"
#include "Team8_Project/MyCharacter.h"

void UInfinityAmmoEffect::ApplyItemEffect(AActor* Target)
{
    if (Target)
    {
        if (AMyCharacter* player = Cast<AMyCharacter>(Target))
        {
            //TODO :: CombatComponent Method
        }
    }
}
