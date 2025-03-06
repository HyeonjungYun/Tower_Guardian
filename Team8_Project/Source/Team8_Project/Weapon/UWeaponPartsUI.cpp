// Fill out your copyright notice in the Description page of Project Settings.


#include "UWeaponPartsUI.h"
#include "../MyPlayerController.h"

#include "WeaponBase.h"
#include "PlayerCombatComponent.h"

void UUWeaponPartsUI::InitializeAttachmentUI(AWeaponBase* InWeaponBase)
{
    MyWeaponBase = InWeaponBase;

    if (!bIsOnceUICreated)
    {
        // 각 부착물 카테고리별 UI 생성
        CreateAttachmentImages(MyWeaponBase->OpticWeaponpartsTableRows);
        CreateAttachmentImages(MyWeaponBase->MagzineWeaponpartsTableRows);
        CreateAttachmentImages(MyWeaponBase->GripWeaponpartsTableRows);
        CreateAttachmentImages(MyWeaponBase->MuzzleWeaponpartsTableRows);

        bIsOnceUICreated = true;
    }

    UpdateHighlight();
}

void UUWeaponPartsUI::SelectUp()
{
    CurrentOpticIndex = (CurrentOpticIndex + 1) % OpticPartImages.Num();

    UpdateHighlight();
    WeaponAttachmentUpdate(EWeaponPartsType::EWT_Optic, CurrentOpticIndex);
}

void UUWeaponPartsUI::SelectDown()
{
    CurrentMagazineIndex = (CurrentMagazineIndex + 1 ) % MagazinePartImages.Num();
    UpdateHighlight();
    WeaponAttachmentUpdate(EWeaponPartsType::EWT_Magazine, CurrentMagazineIndex);
}

void UUWeaponPartsUI::SelectLeft()
{
    CurrentGripIndex = (CurrentGripIndex + 1) % GripPartImages.Num();
    UpdateHighlight();
    WeaponAttachmentUpdate(EWeaponPartsType::EWT_Grip, CurrentGripIndex);
}

void UUWeaponPartsUI::SelectRight()
{
    CurrentMuzzleIndex = (CurrentMuzzleIndex + 1) % MuzzlePartImages.Num();
    UpdateHighlight();
    WeaponAttachmentUpdate(EWeaponPartsType::EWT_Muzzle, CurrentMuzzleIndex);
}

void UUWeaponPartsUI::UpdateHighlight()
{
    for (int32 i = 0; i < OpticPartImages.Num(); i++)
        OpticPartImages[i]->SetRenderOpacity(i == CurrentOpticIndex ? 1.0f : 0.7f);

    for (int32 i = 0; i < MagazinePartImages.Num(); i++)
        MagazinePartImages[i]->SetRenderOpacity(i == CurrentMagazineIndex ? 1.0f : 0.7f);

    for (int32 i = 0; i < GripPartImages.Num(); i++)
        GripPartImages[i]->SetRenderOpacity(i == CurrentGripIndex ? 1.0f : 0.7f);

    for (int32 i = 0; i < MuzzlePartImages.Num(); i++)
        MuzzlePartImages[i]->SetRenderOpacity(i == CurrentMuzzleIndex ? 1.0f : 0.7f);
}

void UUWeaponPartsUI::WeaponAttachmentUpdate(EWeaponPartsType PartType,int32 index)
{
    FName ItemKey;
    switch (PartType)
    {
    case EWeaponPartsType::EWT_Optic:
        ItemKey = OpticPartItemKey[index];
        break;
    case EWeaponPartsType::EWT_Grip:
        ItemKey = GripPartItemKey[index];
        break;
    case EWeaponPartsType::EWT_Magazine:
        ItemKey = MagazinePartItemKey[index];
        break;
    case EWeaponPartsType::EWT_Muzzle:
        ItemKey = MuzzlePartItemKey[index];
        break;
    default:
        return;
    }
    MyWeaponBase->EquipWeaponPart(PartType, ItemKey);
}
