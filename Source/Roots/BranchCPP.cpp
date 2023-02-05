// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "ResourceSetFunctions.h"
#include <Roots/RootsGameState.h>

// Sets default values
UBranchCPP::UBranchCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}


void UBranchCPP::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBranchCPP::OnRegister()
{
	Super::OnRegister();
	CleanSegments();
	if (Segments.Num() > 0)
	{
		Segments[0]->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	for (int i = 0; i < Segments.Num() - 1; i++)
	{
		Segments[i + 1]->AttachToComponent(Segments[i], FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}


// Called when the game starts or when spawned
void UBranchCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UBranchCPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBranchCPP::AlignSegments()
{
	CleanSegments();
	if (!Segments.IsEmpty())
	{
		FVector CurrentWorldLocation = GetComponentLocation();
		Segments[0]->SetWorldLocation(CurrentWorldLocation);
		Segments[0]->SetWorldRotation(Segments[0]->SegmentDirection);
		for (int i = 1; i < Segments.Num(); i++)
		{
			UBranchSegmentCPP* PreviousSegment = Segments[i - 1];
			FVector PreviousOffset = PreviousSegment->Length * (PreviousSegment->SegmentDirection.RotateVector(FVector(0, 0, 1)));
			UBranchSegmentCPP* Segment = Segments[i];
			CurrentWorldLocation += PreviousOffset;
			Segment->SetWorldLocation(CurrentWorldLocation);
			Segment->SetWorldRotation(Segment->SegmentDirection);
		}
	}
	for (UBranchSegmentCPP* Segment : Segments)
	{
		for (UBranchCPP* Branch : Segment->ConnectedBranches)
		{
			Branch->AlignSegments();
		}
	}

}

void UBranchCPP::GenerateConnectionPoints()
{
	CleanSegments();
	for (UBranchSegmentCPP* Segment : Segments)
	{
		Segment->GenerateConnectionPoints();
	}
}


void UBranchCPP::CleanSegments()
{
	TArray<UBranchSegmentCPP*> NewSegments;
	for (UBranchSegmentCPP* Segment : Segments)
	{
		if (IsValid(Segment))
		{
			NewSegments.Add(Segment);
		}
	}
	Segments = NewSegments;
}

FRotator UBranchCPP::GetGrowDirection()
{
	if (Segments.IsEmpty())
	{
		return GetComponentRotation();
	}
	return  Segments.Last()->GrowDirectionWorld;
}

FResourceSet UBranchCPP::GatherResources()
{
	CleanSegments();
	FResourceSet Result = FResourceSet();
	for (UBranchSegmentCPP* Segment : Segments)
	{
		FResourceSet GatheredFromSegment = Segment->GatherResources();
		Result.Water += GatheredFromSegment.Water;
		Result.Nutrients += GatheredFromSegment.Nutrients;
	}
	return Result;
}

float UBranchCPP::GetLength()
{
	float AccumulatedLength = 0;
	for (UBranchSegmentCPP* Segment : Segments)
	{
		AccumulatedLength += Segment->GetSubLength();
		AccumulatedLength += Segment->GetSegmentLength();
	}
	return AccumulatedLength;
}

TArray<UBranchSegmentCPP*> UBranchCPP::GetAllSubSegments()
{
	TArray<UBranchSegmentCPP*> Result = TArray<UBranchSegmentCPP*>();
	for (UBranchSegmentCPP* Segment : Segments)
	{
		Result.Add(Segment);
		for (UBranchCPP* Branch : Segment->ConnectedBranches)
		{
			Result += Branch->GetAllSubSegments();
		}
	}
	return Result;
}

FResourceSet UBranchCPP::Grow(FResourceSet InputResources)
{
	FResourceSet ResultSet = InputResources;
	FResourceSet ResourcesNeededForNewSegment = GetResourcesNeededForNewSegment();
	if (CanSupportNewSegmentAtEnd() && UResourceSetFunctions::HasResources(ResultSet, ResourcesNeededForNewSegment))
	{
		ResultSet = UResourceSetFunctions::DrainResources(ResultSet, ResourcesNeededForNewSegment);
		AddNewSegment(GetGrowDirection());
	}
	for (UBranchSegmentCPP* Segment : Segments)
	{
		ResultSet = Segment->Grow(ResultSet);
	}
	FResourceSet ResourcesNeededToGrowSelf = GetGrowthCost();
	if (CanGrowSelf() && UResourceSetFunctions::HasResources(ResultSet, ResourcesNeededToGrowSelf))
	{
		ResultSet = UResourceSetFunctions::DrainResources(ResultSet, ResourcesNeededToGrowSelf);
		GrowSelf();
	}
	return ResultSet;
}

bool UBranchCPP::CanSupportNewSegmentAtEnd()
{
	float ThresholdLength = MaxSegmentLength * 0.7 * Segments.Num();
	return Segments.Num() < MaxNSegments && GetLength() > ThresholdLength;
}

FResourceSet UBranchCPP::GetResourcesNeededForNewSegment()
{
	if (UWorld* World = GetWorld())
	{
		ARootsGameState* const GameState = World->GetGameState<ARootsGameState>();
		if (IsValid(GameState))
		{
			return GameState->CostToGrowNewSegment;
		}
	}
	return FResourceSet(10, 10);
}

bool UBranchCPP::CanGrowSelf()
{
	return GetLength() < MaxSegmentLength * Segments.Num();
}

FResourceSet UBranchCPP::GetGrowthCost()
{
	int NSegmentsThatCanGrow = 0;
	for (UBranchSegmentCPP* Segment : Segments)
	{
		if (Segment->GetSegmentLength() < MaxSegmentLength)
		{
			NSegmentsThatCanGrow += 1;
		}
	}
	float SingleSegmentWaterCost = 1;
	float SingleSegmentNutrientCost = 1;
	if (UWorld* World = GetWorld())
	{
		ARootsGameState* const GameState = World->GetGameState<ARootsGameState>();
		if (IsValid(GameState))
		{
			FResourceSet CostForGrowingSegment = GameState->CostToGrowSingleSegment;
			SingleSegmentWaterCost = CostForGrowingSegment.Water;
			SingleSegmentNutrientCost = CostForGrowingSegment.Nutrients;
		}
	}

	return FResourceSet(NSegmentsThatCanGrow * SingleSegmentWaterCost, NSegmentsThatCanGrow * SingleSegmentNutrientCost);
}

void UBranchCPP::GrowSelf()
{
	if (Segments.IsEmpty())
	{
		return;
	}
	/*if (Segments.Last()->GetSegmentLength() < MaxSegmentLength)
	{
		Segments.Last()->SegmentDirection = Segments.Last()->GrowDirectionWorld;// .RotateVector(FVector::UpVector).ToOrientationRotator();
		Segments.Last()->OnConfigurationChanged();
	}*/
	for (int i = 0; i < Segments.Num(); i++)
	{
		UBranchSegmentCPP* Segment = Segments[i];
		UBranchSegmentCPP* PreviousSegment = i == 0 ? nullptr : Segments[i - 1];
		FVector GrowthOffset = FVector(0, 0, 0);
		FVector InitialEndLocation = Segment->GetEndLocation();
		if (Segment->GetSegmentLength() < MaxSegmentLength)
		{
			float NewLength = FMath::Clamp(Segment->Length * 1.1f, 0, MaxSegmentLength);
			Segment->SetLength(NewLength);
			if (i == 0)
			{
				Segment->StartRadius = Segment->StartRadius * 1.1;
			}
		}
		FVector GrowthOffsetAtEnd = Segment->GetEndLocation() - InitialEndLocation;
		Segment->EndRadius = Segment->StartRadius * 0.8;
		if (i < Segments.Num() - 1)
		{
			Segments[i + 1]->AddWorldOffset(GrowthOffsetAtEnd);
			Segments[i + 1]->StartRadius = Segment->EndRadius;
		}
	}
}

UBranchSegmentCPP* UBranchCPP::GetRandomSubSegment()
{
	TArray<UBranchSegmentCPP*> AllSubSegments = GetAllSubSegments();
	if (AllSubSegments.IsEmpty())
	{
		return nullptr;
	}
	return AllSubSegments[FMath::RandRange(0, AllSubSegments.Num() - 1)];
}

UBranchSegmentCPP* UBranchCPP::AddNewSegmentConstructor(FRotator Direction)
{
	CleanSegments();
	if (Segments.Num() >= MaxNSegments)
	{
		return nullptr;
	}
	if (!Segments.IsEmpty())
	{
		Segments.Last()->bPointerMeshDisabled = true;
	}
	UBranchSegmentCPP* NewSegment = CreateDefaultSubobject<UBranchSegmentCPP>(FName(FString::Printf(TEXT("Segment%d"), Segments.Num())));
	NewSegment->Length = MaxSegmentLength / 2;
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	FRotator NewDirection = Direction + RandomVariance;
	NewSegment->GrowDirectionWorld = NewDirection;
	NewSegment->SegmentDirection = NewDirection;
	NewSegment->SetWorldRotation(NewDirection);
	Segments.Add(NewSegment);
	return NewSegment;
}

UBranchSegmentCPP* UBranchCPP::AddNewSegment(FRotator Direction)
{
	CleanSegments();
	if (Segments.Num() >= MaxNSegments)
	{
		return nullptr;
	}
	if (!Segments.IsEmpty())
	{
		Segments.Last()->bPointerMeshDisabled = true;
	}
	UBranchSegmentCPP* NewSegment = NewObject<UBranchSegmentCPP>(this, UBranchSegmentCPP::StaticClass(), FName(FString::Printf(TEXT("Segment%d"), Segments.Num())));
	USceneComponent* AttachToComponent;
	if (Segments.Num() > 0)
	{
		UBranchSegmentCPP* AttachedToSegment = Segments.Last();
		NewSegment->StartRadius = AttachedToSegment->EndRadius;
		NewSegment->EndRadius = NewSegment->StartRadius * 0.8;
		AttachToComponent = AttachedToSegment;
	}
	else
	{
		AttachToComponent = this;
	}
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	FRotator NewDirection = Direction + RandomVariance;
	NewDirection = NewDirection.Clamp();
	// NewDirection.Yaw -= FMath::FRand() * (NewDirection.Yaw - 180);
	NewSegment->GrowDirectionWorld = NewDirection;
	NewSegment->SegmentDirection = NewDirection;
	NewSegment->SetWorldRotation(NewDirection);
	NewSegment->SetLength(MaxSegmentLength / 2);
	NewSegment->RegisterComponent();
	NewSegment->AttachToComponent(AttachToComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewSegment->OnConfigurationChanged();
	Segments.Add(NewSegment);
	return NewSegment;
}