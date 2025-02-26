// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperAmmoItem.h"

ASniperAmmoItem::ASniperAmmoItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	SlotType = EItemType::Ammo;
	FItemType = "SniperAmmo";
}