﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItem.h"

AConsumableItem::AConsumableItem()
{
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(GetRootComponent());
}