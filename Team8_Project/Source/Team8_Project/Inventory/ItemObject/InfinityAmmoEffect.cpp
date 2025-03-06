// Fill out your copyright notice in the Description page of Project Settings.


#include "InfinityAmmoEffect.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/Weapon/PlayerCombatComponent.h"
#include "Team8_Project/GameState/CH8_GameState.h"

void UInfinityAmmoEffect::ApplyItemEffect(AActor* Target)
{
    if (Target)
    {
        if (AMyCharacter* Player = Cast<AMyCharacter>(Target))
        {
           
            Player->GetCombatComponent()->OnInfiniteAmmoStart(Duration);
            ACH8_GameState* GameStateRef = Cast<ACH8_GameState>(GetWorld()->GetGameState());
            GameStateRef->UseInfinityAmmoItem();
        }
    }
}
