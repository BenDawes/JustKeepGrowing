// Fill out your copyright notice in the Description page of Project Settings.


#include "RootsSystemCPP.h"
#include "BranchCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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
	CostToGrowSingleSegment = FResourceSet(5, 5);
	CostToGrowNewNub = FResourceSet(50, 50);
	CostToGrowNewSegment = FResourceSet(25, 25);

	StartingResources = FResourceSet(30, 30);
	StoredResources->SetResources(StartingResources);

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
			GameState->CostToGrowNewNub = CostToGrowNewNub;
			GameState->CostToGrowSingleSegment = CostToGrowSingleSegment;
			GameState->CostToGrowNewSegment = CostToGrowNewSegment;
		}
		FVector SpawnLocation = FVector(0, 0, -FMath::FRandRange(400.f, 1000.f));
		if (RootBranch->Segments.IsEmpty())
		{
			return;
		}
		if (AEarthCPP* Earth = Cast<AEarthCPP>(UGameplayStatics::GetActorOfClass(World, AEarthCPP::StaticClass())))
		{
			World->SpawnActor<ANutrientPocketCPP>(Earth->NutrientPocketClass.Get(), FTransform(GetActorLocation() + FVector(0, 0, -100)));
			if (!RootBranch->Segments.IsEmpty())
			{
				World->SpawnActor<ANutrientPocketCPP>(Earth->NutrientPocketClass.Get(), FTransform(RootBranch->Segments.Last()->GetComponentLocation()));
			}
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
	RootBranch->AlignSegments();
	OnGrowFinished();
}

FResourceSet ARootsSystemCPP::GatherResources()
{
	FResourceSet Result = RootBranch->GatherResources();
	Result = CapResources(Result);
	StoredResources->AddResources(Result);
	return Result;
}
/*
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
*/
FResourceSet ARootsSystemCPP::CapResources(FResourceSet InResources)
{
	int NSegments = RootBranch->GetAllSubSegments().Num();

	float MaxWater = NSegments * 100;
	float MaxNutrients = NSegments * 100;

	return FResourceSet(FMath::Clamp(InResources.Water, 0, MaxWater), FMath::Clamp(InResources.Nutrients, 0, MaxNutrients));
}