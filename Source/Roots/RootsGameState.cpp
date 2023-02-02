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

TArray<TWeakObjectPtr<ARootsSystemCPP>> ARootsGameState::GetRootsSystems()
{
	return RootsSystems;
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
