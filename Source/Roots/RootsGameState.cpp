// Fill out your copyright notice in the Description page of Project Settings.


#include "RootsGameState.h"

ARootsGameState::ARootsGameState()
{
	WeatherSettings = FWeatherSettings();
	WeatherSettings.MaxRainfalls.Add(
		ESeason::AUTUMN, 20.f
	);
	WeatherSettings.MaxRainfalls.Add(
		ESeason::WINTER, 12.f
	);
	WeatherSettings.MaxRainfalls.Add(
		ESeason::SPRING, 19.f
	);
	WeatherSettings.MaxRainfalls.Add(
		ESeason::SUMMER, 10.f
	);
	WeatherSettings.MinRainfalls.Add(
		ESeason::AUTUMN, 14.f
	);
	WeatherSettings.MinRainfalls.Add(
		ESeason::WINTER, 2.f
	);
	WeatherSettings.MinRainfalls.Add(
		ESeason::SPRING, 10.f
	);
	WeatherSettings.MinRainfalls.Add(
		ESeason::SUMMER, 2.f
	);
	WeatherSettings.MinTemperatures.Add(
		ESeason::AUTUMN, 4.f
	);
	WeatherSettings.MinTemperatures.Add(
		ESeason::WINTER, -2.f
	);
	WeatherSettings.MinTemperatures.Add(
		ESeason::SPRING, 8.f
	);
	WeatherSettings.MinTemperatures.Add(
		ESeason::SUMMER, 14.f
	);
	WeatherSettings.MaxTemperatures.Add(
		ESeason::AUTUMN, 15.f
	);
	WeatherSettings.MaxTemperatures.Add(
		ESeason::WINTER, 3.f
	);
	WeatherSettings.MaxTemperatures.Add(
		ESeason::SPRING, 27.f
	);
	WeatherSettings.MaxTemperatures.Add(
		ESeason::SUMMER, 42.f
	);
}

void ARootsGameState::BeginPlay()
{
	EnterState(ERootsTurnState::PLAYING_SEASON);
	EnterState(ERootsTurnState::ASSIGN_MOVES);
}

void ARootsGameState::AddRootsSystems(TArray<ARootsSystemCPP*> NewRootsSystems)
{
	for (ARootsSystemCPP* RootSystem : NewRootsSystems)
	{
		if (IsValid(RootSystem))
		{
			RootsSystems.AddUnique(RootSystem);
		}
	}
}

TArray<ARootsSystemCPP*> ARootsGameState::GetRootsSystems()
{
	TArray<ARootsSystemCPP*> Result;
	for (TWeakObjectPtr<ARootsSystemCPP> RootSystem : RootsSystems)
	{
		if (RootSystem.IsValid())
		{
			Result.Add(RootSystem.Get());
		}
	}
	return Result;
}

void ARootsGameState::EnterState(ERootsTurnState NewState)
{
	TurnState = NewState;
	switch (NewState)
	{
	case ERootsTurnState::GATHERING_RESOURCES:
	{
		BeginGatherResources();
		break;
	}
	case ERootsTurnState::SPENDING_RESOURCES:
	{
		BeginSpendResources();
		break;

	}
	case ERootsTurnState::PLAYING_SEASON:
	{
		BeginPlaySeason();
		break;
	}
	case ERootsTurnState::ASSIGN_MOVES:
	default:
	{
		BeginAssignMoves();
		break;
	}
	}
}

void ARootsGameState::BeginGatherResources()
{
	for (TWeakObjectPtr<ARootsSystemCPP> RootsSystem : RootsSystems)
	{
		if (RootsSystem.IsValid())
		{
			RootsSystem->GatherResources();
		}
	}
	// TODO: Destroy empty nutrient pockets
}

void ARootsGameState::BeginSpendResources()
{
	for (TWeakObjectPtr<ARootsSystemCPP> RootsSystem : RootsSystems)
	{
		if (RootsSystem.IsValid())
		{
			RootsSystem->Grow();
		}
	}
}

void ARootsGameState::BeginPlaySeason()
{
	if (!Earth.IsValid())
	{
		return;
	}
	Earth->UpdateAllMoisture(NextSeasonTemperature, NextSeasonRainfall);
	Season = NextSeason;
	switch (Season) {
	case ESeason::WINTER: {
		NextSeason = ESeason::SPRING;
		break;
	}
	case ESeason::SPRING: {
		NextSeason = ESeason::SUMMER;
		break;
	}
	case ESeason::SUMMER: {
		NextSeason = ESeason::AUTUMN;
		break;
	}
	case ESeason::AUTUMN:
	default: {
		NextSeason = ESeason::WINTER;
		break;
	}
	}
	NextSeasonTemperature = FMath::FRandRange(*WeatherSettings.MinTemperatures.Find(NextSeason), *WeatherSettings.MaxTemperatures.Find(NextSeason));
	NextSeasonRainfall = FMath::FRandRange(*WeatherSettings.MinRainfalls.Find(NextSeason), *WeatherSettings.MaxRainfalls.Find(NextSeason));
}

void ARootsGameState::BeginAssignMoves()
{
}

void ARootsGameState::PlaySeason()
{
	EnterState(ERootsTurnState::GATHERING_RESOURCES);
	EnterState(ERootsTurnState::SPENDING_RESOURCES);
	EnterState(ERootsTurnState::PLAYING_SEASON);
	EnterState(ERootsTurnState::ASSIGN_MOVES);
}
