// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleDamagableActor.h"

// Sets default values
ASampleDamagableActor::ASampleDamagableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SimpleActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SimpleActor"));
	SetRootComponent(SimpleActor);
}

float ASampleDamagableActor::GetHP() const
{
	return Health;
}

void ASampleDamagableActor::SetHP(float Value)
{
	Health -= Value;

	if (Health<=0)
	{
		OnDeath();
	}
}

float ASampleDamagableActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 상위 클래스에서 다른 데미지 처리 공식이 있다면
	// float Dmg = Super::TakeDamage 호출

	Health -= DamageAmount;

	if (Health <= 0)
	{
		OnDeath();
	}
	return DamageAmount;
}


void ASampleDamagableActor::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("샘플 액터 사망"));
	Destroy();
}

// Called when the game starts or when spawned
void ASampleDamagableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASampleDamagableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

