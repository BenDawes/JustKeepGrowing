// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RootsTurnState.h"
#include "RootsSystemCPP.h"
#include "RootsGameState.generated.h"

/**
 * 
 */
UCLASS()
class ROOTS_API ARootsGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

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
	
};
