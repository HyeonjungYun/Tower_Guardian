// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedItemEffect.h"
#include "Team8_Project/MyCharacter.h"

void USpeedItemEffect::ApplyItemEffect(AActor* Target)
{
    if (Target)
    {
        if (AMyCharacter* player = Cast<AMyCharacter>(Target))
        {
            player->ApplySpeedBoost(SpeedAmount, Duration);
        }
    }
}
