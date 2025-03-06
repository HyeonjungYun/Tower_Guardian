// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "OpticWeaponPartsTable.h"
#include "MuzzleWeaponPartsTable.h"
#include "GripWeaponPartsTable.h"
#include "MagazineWeaponPartsTable.h"
#include "Components/PanelWidget.h"
#include "WeaponPartsType.h"
#include "UWeaponPartsUI.generated.h"



class AWeaponBase;
class UImage;
class UVerticalBox;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class TEAM8_PROJECT_API UUWeaponPartsUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void InitializeAttachmentUI(AWeaponBase* InWeaponBase);

    template <typename T>
    void CreateAttachmentImages(const TArray<T*>& TableRows)
    {
        if (!VerticalBox_Optic || !VerticalBox_Magazine || !HorizontalBox_Grip || !HorizontalBox_Muzzle)
        {
            return;
        }
        for (T* Row : TableRows)
        {
            if (!Row || !Row->ItemImage) continue;


            USizeBox* SizeBox = NewObject<USizeBox>(this);
            SizeBox->SetWidthOverride(100.0f);
            SizeBox->SetHeightOverride(100.0f);

            UImage* NewImage = NewObject<UImage>(this);
            NewImage->SetBrushFromTexture(Row->ItemImage);
            NewImage->SetVisibility(ESlateVisibility::Visible);
            NewImage->SetBrushSize(FVector2D(90, 90)); // 정사각형 유지

            // SizeBox에 Image 추가
            SizeBox->AddChild(NewImage);

            // 해당 무기 부착물 유형에 따라 적절한 박스에 추가
            switch (Row->WeaponPartsType)
            {
            case EWeaponPartsType::EWT_Optic:
                VerticalBox_Optic->AddChild(SizeBox);
                OpticPartImages.Add(NewImage);
                OpticPartItemKey.Add(Row->ItemKey);
                break;

            case EWeaponPartsType::EWT_Magazine:
                VerticalBox_Magazine->AddChild(SizeBox);
                MagazinePartImages.Add(NewImage);
                MagazinePartItemKey.Add(Row->ItemKey);
                break;

            case EWeaponPartsType::EWT_Grip:
                HorizontalBox_Grip->AddChild(SizeBox);
                GripPartImages.Add(NewImage);
                GripPartItemKey.Add(Row->ItemKey);
                break;

            case EWeaponPartsType::EWT_Muzzle:
                HorizontalBox_Muzzle->AddChild(SizeBox);
                MuzzlePartImages.Add(NewImage);
                MuzzlePartItemKey.Add(Row->ItemKey);
                break;

            default:
                break;
            }
        }
    }
    // UI 조작 함수
    UFUNCTION(BlueprintCallable)
    void SelectUp();
    UFUNCTION(BlueprintCallable)
    void SelectDown();
    UFUNCTION(BlueprintCallable)
    void SelectLeft();
    UFUNCTION(BlueprintCallable)
    void SelectRight();
    UFUNCTION(BlueprintCallable)
    void UpdateSelection() {}; // 현재 선택한 부착물 활성화
    UFUNCTION(BlueprintCallable)
    void UpdateHighlight(); // 하이라이트 UI 업데이트

    void AttachSelectedPart();

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VerticalBox_Optic;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VerticalBox_Magazine;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* HorizontalBox_Grip;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* HorizontalBox_Muzzle;


    AWeaponBase* MyWeaponBase;

    // 각 카테고리별 현재 선택된 인덱스 유지
    int32 CurrentOpticIndex = 0;
    int32 CurrentMagazineIndex = 0;
    int32 CurrentGripIndex = 0;
    int32 CurrentMuzzleIndex = 0;
    // 
    TArray<UImage*> OpticPartImages;
    TArray<UImage*> MagazinePartImages;
    TArray<UImage*> GripPartImages;
    TArray<UImage*> MuzzlePartImages;

    TArray<FName> OpticPartItemKey;
    TArray<FName> MagazinePartItemKey;
    TArray<FName> GripPartItemKey;
    TArray<FName> MuzzlePartItemKey;

    bool bIsOnceUICreated = false;

    void WeaponAttachmentUpdate(EWeaponPartsType PartType, int32 index);

};
