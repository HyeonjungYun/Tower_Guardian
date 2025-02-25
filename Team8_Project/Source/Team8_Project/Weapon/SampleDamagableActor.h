// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Damageable.h"
#include "SampleDamagableActor.generated.h"

UCLASS()
class TEAM8_PROJECT_API ASampleDamagableActor : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASampleDamagableActor();
public:
	UFUNCTION(BlueprintCallable)
	virtual float GetHP() const;
	UFUNCTION(BlueprintCallable)
	virtual void SetHP(float Value);
	
	UFUNCTION(BlueprintCallable)
	virtual void Damage(float _dmg);

	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SimpleActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
