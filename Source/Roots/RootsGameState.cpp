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
	for (ARootsSystemCPP* RootsSystem : RootsSystems)
	{
		RootsSystem->GatherResources();
	}
	// TODO: Destroy empty nutrient pockets
}

void ARootsGameState::BeginSpendResources()
{
	for (ARootsSystemCPP* RootsSystem : RootsSystems)
	{
		RootsSystem->Grow();
	}
}

void ARootsGameState::BeginPlaySeason()
{
}

void ARootsGameState::BeginAssignMoves()
{
}
