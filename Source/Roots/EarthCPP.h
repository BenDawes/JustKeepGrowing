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
		float GetWater(FVector StartLocation, FVector EndLocation, float Radius, float Rate);

	UFUNCTION(BlueprintCallable)
	void UpdateAllMoisture(float Temperature, float Rainfall);

	UFUNCTION(BlueprintCallable)
	float DrainNutrients(FVector StartLocation, FVector EndLocation, float Radius, float Rate);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MoistureLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxRainfall;
};
