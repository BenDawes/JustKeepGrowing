// Fill out your copyright notice in the Description page of Project Settings.


#include "RootsSystemCPP.h"
#include "BranchCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "RootsGameState.h"

// Sets default values
ARootsSystemCPP::ARootsSystemCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Radius = 500;
	CachedRadius = Radius;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootBranch = CreateDefaultSubobject<UBranchCPP>("RootBranch");
	StoredResources = CreateDefaultSubobject<UResourceContainer>("StoredResources");
	RootBranch->SetupAttachment(RootComponent);
	RootBranch->SetWorldRotation(FRotator(180, 0, 0));
}

void ARootsSystemCPP::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UBranchSegmentCPP* FirstSegment = RootBranch->Segments.IsEmpty() ? nullptr : RootBranch->Segments[0];
	if (IsValid(FirstSegment))
	{
		FirstSegment->StartRadius = 70;
		FirstSegment->EndRadius = FirstSegment->StartRadius * 0.8;
		FirstSegment->SetLength(230);
	}
}

// Called when the game starts or when spawned
void ARootsSystemCPP::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		ARootsGameState* const GameState = World->GetGameState<ARootsGameState>();
		if (IsValid(GameState))
		{
			TArray<ARootsSystemCPP*> InputRoots;
			InputRoots.Add(this);
			GameState->AddRootsSystems(InputRoots);
		}
	}
}

// Called every frame
void ARootsSystemCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARootsSystemCPP::Grow()
{
	StoredResources->SetResources(RootBranch->Grow(StoredResources->Resources));
}

FResourceSet ARootsSystemCPP::GatherResources()
{
	FResourceSet Result = RootBranch->GatherResources();
	StoredResources->AddResources(Result);
	return Result;
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