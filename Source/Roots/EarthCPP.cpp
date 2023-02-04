// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthCPP.h"
#include "NutrientPocketCPP.h"

// Sets default values
AEarthCPP::AEarthCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoistureLevel = 0.5f;
	MaxRainfall = 20.f;
}

// Called when the game starts or when spawned
void AEarthCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEarthCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEarthCPP::GetWater(FVector StartLocation, FVector EndLocation, float Radius, float Rate)
{
	// TODO Returns a %ge from 0 to 1
	// TODO Sample from texture?
	return MoistureLevel * (EndLocation - StartLocation).Length() * Rate * 0.001f;
}

void AEarthCPP::UpdateAllMoisture(float Temperature, float Rainfall)
{
	// TODO Add rainfall, if temp < 0 or > 25 scale rainfall down by some factor
	// Multiply by a porousness texture?

	if (Temperature > 25)
	{
		MoistureLevel *= 0.8f;
	}

	float IncomingRainfallMultiplier = 1.f + (Rainfall / MaxRainfall) * (Temperature < 0 ? 0.5f : 1.f);
	MoistureLevel *= IncomingRainfallMultiplier;
	MoistureLevel = FMath::Clamp(MoistureLevel, 0.1f, 0.9f);
}

float AEarthCPP::DrainNutrients(FVector StartLocation, FVector EndLocation, float Radius, float Rate)
{
	// TODO Get nutrient actors this location is inside of
	// Reduce them by some amount, return that amount
	// They get destroyed if reduced to 0
	float AccumulatedNutrients = 0;
	if (UWorld* World = GetWorld())
	{
		TArray<FHitResult> OutHits;
		World->SweepMultiByChannel(OutHits, StartLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(Radius));
		for (FHitResult Hit : OutHits)
		{
			ANutrientPocketCPP* AsPocket = Cast<ANutrientPocketCPP>(Hit.GetActor());
			if (!IsValid(AsPocket))
			{
				continue;
			}
			AccumulatedNutrients += AsPocket->DrainNutrients(Rate);
			if (AsPocket->RemainingNutrients <= 0.1f)
			{
				AsPocket->Destroy();
			}
		}
	}
	return AccumulatedNutrients;
}

