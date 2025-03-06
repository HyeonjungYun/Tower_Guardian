// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedItemEffect.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/GameState/CH8_GameState.h"

void USpeedItemEffect::ApplyItemEffect(AActor* Target)
{
    if (Target)
    {
        if (AMyCharacter* player = Cast<AMyCharacter>(Target))
        {
            player->ApplySpeedBoost(SpeedAmount, Duration);
            ACH8_GameState* GameStateRef = Cast<ACH8_GameState>(GetWorld()->GetGameState());
            GameStateRef->UseHeistItem();
        }
    }
}
