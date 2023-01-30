// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceSetFunctions.h"

FResourceSet UResourceSetFunctions::AddWater(FResourceSet InResourceSet, float IncomingWater)
{
	FResourceSet Result = FResourceSet(InResourceSet);
	Result.Water += IncomingWater;
	return Result;
}

FResourceSet UResourceSetFunctions::AddNutrients(FResourceSet InResourceSet, float IncomingNutrients)
{
	FResourceSet Result = FResourceSet(InResourceSet);
	Result.Nutrients += IncomingNutrients;
	return Result;
}

bool UResourceSetFunctions::HasResources(FResourceSet InResourceSet, FResourceSet CheckAmount)
{
	return HasWater(InResourceSet, CheckAmount.Water) && HasNutrients(InResourceSet, CheckAmount.Nutrients);
}

FResourceSet UResourceSetFunctions::DrainWater(FResourceSet InResourceSet, float DesiredWater)
{
	FResourceSet Result = FResourceSet(InResourceSet);
	Result.Water = FMath::Clamp(InResourceSet.Water - DesiredWater, 0, InResourceSet.Water);
	return Result;
}

FResourceSet UResourceSetFunctions::DrainNutrients(FResourceSet InResourceSet, float DesiredNutrients)
{
	FResourceSet Result = FResourceSet(InResourceSet);
	Result.Nutrients = FMath::Clamp(InResourceSet.Nutrients - DesiredNutrients, 0, InResourceSet.Nutrients);
	return Result;
}

bool UResourceSetFunctions::HasWater(FResourceSet InResourceSet, float CheckAmount)
{
	return InResourceSet.Water >= CheckAmount;
}

bool UResourceSetFunctions::HasNutrients(FResourceSet InResourceSet, float CheckAmount)
{
	return InResourceSet.Nutrients >= CheckAmount;
}

FResourceSet UResourceSetFunctions::DrainResources(FResourceSet InResourceSet, FResourceSet DesiredAmounts)
{
	return DrainWater(DrainNutrients(InResourceSet, DesiredAmounts.Nutrients), DesiredAmounts.Water);
}
