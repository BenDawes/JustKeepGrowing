// Fill out your copyright notice in the Description page of Project Settings.


#include "RootsGameState.h"

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
	}
	case ERootsTurnState::SPENDING_RESOURCES:
	{
		BeginSpendResources();

	}
	case ERootsTurnState::PLAYING_SEASON:
	{
		BeginPlaySeason();
	}
	case ERootsTurnState::ASSIGN_MOVES:
	default:
	{
		BeginAssignMoves();
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
}

void ARootsGameState::BeginAssignMoves()
{
}
