// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSpawnUISubSystem.h"

#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

UWorldSpawnUISubSystem::UWorldSpawnUISubSystem()
{
	ConstructorHelpers::FClassFinder<AActor> DamageTextWidgetActorClassFinder(
		TEXT("/Game/UI/BP_DamageTextActor"));
	if (DamageTextWidgetActorClassFinder.Succeeded())
		DamageTextWidgetActorClass = DamageTextWidgetActorClassFinder.Class;
}


void UWorldSpawnUISubSystem::SpawnDamageText(UWorld* World, float Damage, const FVector Location)
{
	AActor* TextActor = World->SpawnActor(DamageTextWidgetActorClass, &Location);
	UWidgetComponent* WidgetComp = TextActor->GetComponentByClass<UWidgetComponent>();
	UUserWidget* Widget = WidgetComp->GetWidget();
	UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName("DamageText"));
	
	TextBlock->SetText(FText::FromString(FString::FromInt(Damage)));
	
	TextActor->SetLifeSpan(2.f);
}
