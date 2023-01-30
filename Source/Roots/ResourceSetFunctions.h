// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FResourceSet.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ResourceSetFunctions.generated.h"

/**
 * 
 */
UCLASS()
class ROOTS_API UResourceSetFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UResourceSetFunctions()
	: Super()
	{
	}
public:
		UFUNCTION(BlueprintCallable)
		static FResourceSet AddWater(FResourceSet InResourceSet, float IncomingWater);
	
	UFUNCTION(BlueprintCallable)
		static FResourceSet AddNutrients(FResourceSet InResourceSet, float IncomingNutrients);

	UFUNCTION(BlueprintCallable)
		static FResourceSet DrainWater(FResourceSet InResourceSet, float DesiredWater);

	UFUNCTION(BlueprintCallable)
		static FResourceSet DrainNutrients(FResourceSet InResourceSet, float DesiredNutrients);

	UFUNCTION(BlueprintCallable)
		static bool HasWater(FResourceSet InResourceSet, float CheckAmount);

	UFUNCTION(BlueprintCallable)
		static bool HasNutrients(FResourceSet InResourceSet, float CheckAmount);

	UFUNCTION(BlueprintCallable)
		static FResourceSet DrainResources(FResourceSet InResourceSet, FResourceSet DesiredAmounts);

	UFUNCTION(BlueprintCallable)
		static bool HasResources(FResourceSet InResourceSet, FResourceSet CheckAmount);
};
