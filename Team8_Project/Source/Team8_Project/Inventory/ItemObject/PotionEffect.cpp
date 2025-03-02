// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionEffect.h"
#include "../MyCharacter.h"

void UPotionEffect::ApplyItemEffect(AActor* Target)
{
    if (Target)
    {
        if (AMyCharacter* player = Cast<AMyCharacter>(Target))
        {
            float MaxHealth = player->GetMaxHP();
            float CurrentHealth = player->GetHP();
            
            float Value = MaxHealth * (HealAmount / 100.f);
            float NewHealth = FMath::Min(CurrentHealth + Value, MaxHealth);

            player->SetHP(NewHealth);
            //
        }
    }
}