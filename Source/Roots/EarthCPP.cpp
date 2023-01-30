// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthCPP.h"
#include "NutrientPocketCPP.h"

// Sets default values
AEarthCPP::AEarthCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	// Sample surface moisture texture and scale by depth

	return 0.0f;
}

void AEarthCPP::UpdateAllMoisture(float Temperature, float Rainfall)
{
	// TODO Add rainfall, if temp < 0 or > 25 scale rainfall down by some factor
	// Multiply by a porousness texture
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
		World->SweepMultiByChannel(OutHits, StartLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(Radius));
		for (FHitResult Hit : OutHits)
		{
			ANutrientPocketCPP* AsPocket = Cast<ANutrientPocketCPP>(Hit.GetActor());
			if (!IsValid(AsPocket))
			{
				continue;
			}
			AccumulatedNutrients += AsPocket->DrainNutrients(Rate);
		}
	}
	return AccumulatedNutrients;
}

