// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Season.h"
#include "FWeatherSettings.generated.h"

USTRUCT(BlueprintType)
struct FWeatherSettings
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ESeason, float> MinTemperatures;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<ESeason, float> MaxTemperatures;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<ESeason, float> MinRainfalls;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<ESeason, float> MaxRainfalls;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HotTemperatureThreshold = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MediumHotTemperatureThreshold = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ColdTemperatureThreshold = 2.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HotTemperatureMoistureMultiplierAdjustment = -.6f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MediumTemperatureMoistureMultiplierAdjustment = -.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float DefaultTemperatureMoistureMultiplierAdjustment = -.1f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ColdTemperatureIncomingRainfallMultiplierAdjustment = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxAbsorbedRainfall = 20.f;
	/**/
};