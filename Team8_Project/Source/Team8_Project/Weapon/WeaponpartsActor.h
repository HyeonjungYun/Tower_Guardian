// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPartsType.h"
#include "WeaponpartsActor.generated.h"


UCLASS()
class TEAM8_PROJECT_API AWeaponpartsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponpartsActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Scene;

	// 총기용 USkeletalMeshComponent 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponPartsStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponPartsType WeaponPartsType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponPartsItemKey;

};
