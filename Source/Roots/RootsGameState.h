// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RootsTurnState.h"
#include "RootsSystemCPP.h"
#include "Season.h"
#include "FWeatherSettings.h"
#include "RootsGameState.generated.h"

/**
 * 
 */
UCLASS()
class ROOTS_API ARootsGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ARootsGameState();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void AddRootsSystems(TArray<ARootsSystemCPP*> NewRootsSystems);

	UFUNCTION(BlueprintCallable)
		TArray<ARootsSystemCPP*> GetRootsSystems();

	TArray<TWeakObjectPtr<ARootsSystemCPP>> RootsSystems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERootsTurnState TurnState;

	UFUNCTION(BlueprintCallable)
	void EnterState(ERootsTurnState NewState);

	UFUNCTION(BlueprintCallable)
		void BeginGatherResources();

	UFUNCTION(BlueprintCallable)
		void BeginSpendResources();

	UFUNCTION(BlueprintCallable)
		void BeginPlaySeason();

	UFUNCTION(BlueprintCallable)
		void BeginAssignMoves();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ESeason Season;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ESeason NextSeason;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TWeakObjectPtr<AEarthCPP> Earth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float NextSeasonRainfall;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float NextSeasonTemperature;
	
	UFUNCTION(BlueprintCallable)
		void PlaySeason();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet CostToGrowSingleSegment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet CostToGrowNewSegment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet CostToGrowNewNub;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FWeatherSettings WeatherSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SegmentGatherRate = 10.f;

};
