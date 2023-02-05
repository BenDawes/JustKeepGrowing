// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthCPP.h"
#include "RootsGameState.h"
#include <Kismet/GameplayStatics.h>
#include "NutrientPocketCPP.h"

// Sets default values
AEarthCPP::AEarthCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoistureLevel = 0.5f;
	MaxRainfall = 20.f;
	NutrientPocketClass = ANutrientPocketCPP::StaticClass();
}

// Called when the game starts or when spawned
void AEarthCPP::BeginPlay()
{
	Super::BeginPlay();

	ARootsGameState* const GameState = GetWorld()->GetGameState<ARootsGameState>();
	GameState->Earth = this;

	SpawnNutrients();
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

	ARootsGameState* const GameState = GetWorld()->GetGameState<ARootsGameState>();
	FWeatherSettings WeatherSettings = GameState->WeatherSettings;

	MaxRainfall = WeatherSettings.MaxAbsorbedRainfall;
	float ResultMultiplier = 1.f;
	if (Temperature > WeatherSettings.HotTemperatureThreshold)
	{
		ResultMultiplier += WeatherSettings.HotTemperatureMoistureMultiplierAdjustment;
	}
	else if (Temperature > WeatherSettings.MediumHotTemperatureThreshold)
	{
		ResultMultiplier += WeatherSettings.MediumTemperatureMoistureMultiplierAdjustment;
	}
	else
	{
		ResultMultiplier += WeatherSettings.DefaultTemperatureMoistureMultiplierAdjustment;
	}

	ResultMultiplier += (Rainfall / MaxRainfall) * (Temperature < WeatherSettings.ColdTemperatureThreshold ? WeatherSettings.ColdTemperatureIncomingRainfallMultiplierAdjustment : 1.f);
	MoistureLevel *= ResultMultiplier;
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

void AEarthCPP::SpawnNutrients()
{
	/*TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANutrientPocketCPP::StaticClass(), OutActors);
	for (AActor* Actor : OutActors)
	{
		Actor->Destroy();
	}*/
	int NCellsWidth = 10;
	int NCellsHeight = 10;
	int NCellsLength = 10;

	float GridCellWidth = 1400;
	float GridCellHeight = 900;
	float GridCellLength = 1400;
	for (int i = -NCellsWidth / 2; i < NCellsHeight / 2; i++)
	{
		for (int j = -NCellsLength / 2; j < NCellsLength / 2; j++)
		{
			for (int k = -NCellsHeight / 2; k < NCellsHeight / 2; k++)
			{
				if (i == 0 && j == 0 && k == 0)
				{
					continue;
				}
				FVector SpawnLocation = FVector((i * GridCellWidth) + FMath::FRandRange(0, GridCellWidth), (j * GridCellLength) + FMath::FRandRange(0, GridCellLength), (k * GridCellHeight) + FMath::FRandRange(0, GridCellHeight));
				GetWorld()->SpawnActor<ANutrientPocketCPP>(NutrientPocketClass.Get(), FTransform(SpawnLocation));
			}
		}
	}
}

