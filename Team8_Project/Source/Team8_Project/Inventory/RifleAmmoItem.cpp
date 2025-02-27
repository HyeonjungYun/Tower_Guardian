// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleAmmoItem.h"

ARifleAmmoItem::ARifleAmmoItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	SlotType = EItemType::Ammo;
	FItemType = "RifleAmmo";
	Quantity = 100;
}