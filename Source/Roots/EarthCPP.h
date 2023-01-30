// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EarthCPP.generated.h"

UCLASS()
class ROOTS_API AEarthCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEarthCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetMoisture(FVector AtLocation);

	UFUNCTION(BlueprintCallable)
	void UpdateAllMoisture(float Temperature, float Rainfall);

	UFUNCTION(BlueprintCallable)
	float DrainNutrients(FVector AtLocation);
};
