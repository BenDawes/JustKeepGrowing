// Fill out your copyright notice in the Description page of Project Settings.


#include "RootsSystemCPP.h"
#include "BranchCPP.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARootsSystemCPP::ARootsSystemCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Radius = 500;
	CachedRadius = Radius;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootBranch = CreateDefaultSubobject<UBranchCPP>("RootBranch");
	RootBranch->SetupAttachment(RootComponent);
}

void ARootsSystemCPP::PostInitProperties()
{
	Super::PostInitProperties();
	RootBranch->SetWorldRotation(FRotator(180, 0, 0));
}

// Called when the game starts or when spawned
void ARootsSystemCPP::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARootsSystemCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARootsSystemCPP::Grow(float NutrientsIn)
{
	//
}

float ARootsSystemCPP::GatherWater()
{
	return 0.0f;
}

float ARootsSystemCPP::GatherNutrients()
{
	return 0.0f;
}

void ARootsSystemCPP::PreEditChange(FProperty* PropertyAboutToChange)
{
	if (PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(ARootsSystemCPP, Radius))
	{
		CachedRadius = Radius;
	}
}

void ARootsSystemCPP::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARootsSystemCPP, Radius))
	{
		float DiffPct = Radius / CachedRadius;
	}
	Super::PostEditChangeProperty(e);
}