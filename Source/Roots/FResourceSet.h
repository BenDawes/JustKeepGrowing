// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FResourceSet.generated.h"

USTRUCT(BlueprintType)
struct FResourceSet
{
    GENERATED_BODY()

        FResourceSet() : FResourceSet(0, 0) {};
    FResourceSet(float _Water, float _Nutrients) { Water = _Water; Nutrients = _Nutrients; };


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Water;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Nutrients;


	/**/
};