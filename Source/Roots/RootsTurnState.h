// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERootsTurnState : uint8
{
    ASSIGN_MOVES = 0 UMETA(DisplayName = "Assigning Moves"),
    GATHERING_RESOURCES = 1  UMETA(DisplayName = "Gathering Resources"),
    SPENDING_RESOURCES = 3     UMETA(DisplayName = "Spending Resources"),
    PLAYING_SEASON = 4     UMETA(DisplayName = "Playing Season")
};