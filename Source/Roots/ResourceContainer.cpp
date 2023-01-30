// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceContainer.h"
#include "ResourceSetFunctions.h"
#include "FResourceSet.h"


// Sets default values
UResourceContainer::UResourceContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	Resources = FResourceSet();
}

void UResourceContainer::BeginPlay()
{
	Super::BeginPlay();
}

void UResourceContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UResourceContainer::AddWater(float IncomingWater)
{
	Resources = UResourceSetFunctions::AddWater(Resources, IncomingWater);
	return Resources.Water;
	
}

float UResourceContainer::AddNutrients(float IncomingNutrients)
{
	Resources = UResourceSetFunctions::AddNutrients(Resources, IncomingNutrients);
	return Resources.Nutrients;
}

FResourceSet UResourceContainer::AddResources(FResourceSet IncomingResources)
{
	return FResourceSet(AddWater(IncomingResources.Water), AddNutrients(IncomingResources.Nutrients));
}

bool UResourceContainer::DrainWater(float DesiredWater)
{
	if (UResourceSetFunctions::HasWater(Resources, DesiredWater))
	{
		Resources = UResourceSetFunctions::DrainWater(Resources, DesiredWater);
		return true;
	}
	return false;
}

bool UResourceContainer::DrainNutrients(float DesiredNutrients)
{
	if (UResourceSetFunctions::HasNutrients(Resources, DesiredNutrients))
	{
		Resources = UResourceSetFunctions::DrainNutrients(Resources, DesiredNutrients);
		return true;
	}
	return false;
}

bool UResourceContainer::HasWater(float CheckAmount)
{
	return UResourceSetFunctions::HasWater(Resources, CheckAmount);
}

bool UResourceContainer::HasNutrients(float CheckAmount)
{
	return UResourceSetFunctions::HasNutrients(Resources, CheckAmount);
}

bool UResourceContainer::HasResources(FResourceSet CheckAmount)
{
	return UResourceSetFunctions::HasResources(Resources, CheckAmount);
}

FResourceSet UResourceContainer::DrainResources(FResourceSet DesiredAmounts)
{
	DrainWater(DesiredAmounts.Water);
	DrainNutrients(DesiredAmounts.Nutrients);
	return Resources;
}

void UResourceContainer::SetResources(FResourceSet NewResources)
{
	Resources = NewResources;
}

