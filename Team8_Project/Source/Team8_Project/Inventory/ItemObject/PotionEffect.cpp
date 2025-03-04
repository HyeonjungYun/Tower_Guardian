// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionEffect.h"
#include "Team8_Project/MyCharacter.h"
#include "Team8_Project/Damageable.h"

void UPotionEffect::ApplyItemEffect(AActor* Target)
{
    if (Target)
    {
        if (IDamageable* DamageInterface = Cast<IDamageable>(Target))
        {
           
            float MaxHealth = 100.f;
            float CurrentHealth = DamageInterface->GetHP();
            
            float Value = MaxHealth * (HealAmount / 100.f);
            float NewHealth = FMath::Min(CurrentHealth + Value, MaxHealth);

            DamageInterface->SetHP(NewHealth);
            
        }
    }
}