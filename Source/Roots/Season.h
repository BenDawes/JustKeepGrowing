// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESeason : uint8
{
    WINTER = 0 UMETA(DisplayName = "Winter"),
    SPRING = 1  UMETA(DisplayName = "Spring"),
    SUMMER = 3     UMETA(DisplayName = "Summer"),
    AUTUMN = 4     UMETA(DisplayName = "Autumn")
};