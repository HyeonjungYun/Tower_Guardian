// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponpartsActor.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"
// Sets default values
AWeaponpartsActor::AWeaponpartsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	WeaponPartsStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponParts"));
	WeaponPartsStaticMesh->SetupAttachment(Scene);

}

// Called when the game starts or when spawned
void AWeaponpartsActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponpartsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

