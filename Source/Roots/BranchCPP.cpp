// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "ResourceSetFunctions.h"

// Sets default values
UBranchCPP::UBranchCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
	BaseSegment = CreateDefaultSubobject<UBranchSegmentCPP>("BaseSegment");
	Segments.Add(BaseSegment);
	Nub = CreateDefaultSubobject<UBranchNubCPP>(FName("Nub"));
}


void UBranchCPP::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBranchCPP::OnRegister()
{
	Super::OnRegister();
	BaseSegment->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BaseSegment->SetWorldRotation(GetComponentRotation());
	Nub->AttachToComponent(BaseSegment, FAttachmentTransformRules::SnapToTargetIncludingScale);
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

void UBranchCPP::GenerateConnectionPoints()
{
	for (UBranchSegmentCPP* Segment : Segments)
	{
		if (IsValid(Segment))
		{
			Segment->GenerateConnectionPoints();
		}
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
	if (!IsValid(Nub))
	{
		return FRotator();
	}
	return (Nub->GetComponentLocation() - GetComponentLocation()).ToOrientationRotator() - GetComponentRotation();
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
		AccumulatedLength += Segment->GetLength();
	}
	return AccumulatedLength;
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
		if (Segment->GetLength() < MaxSegmentLength)
		{
			NSegmentsThatCanGrow += 1;
		}
	}
	return FResourceSet(NSegmentsThatCanGrow, NSegmentsThatCanGrow);
}

void UBranchCPP::GrowSelf()
{
	for (int i = 0; i < Segments.Num(); i++)
	{
		UBranchSegmentCPP* Segment = Segments[i];
		UBranchSegmentCPP* PreviousSegment = i == 0 ? nullptr : Segments[i - 1];
		if (Segment->GetLength() < MaxSegmentLength)
		{
			float NewLength = Segment->Length * 1.1f;
			Segment->Length = FMath::Clamp(NewLength, 0, MaxSegmentLength);
			if (i == 0)
			{
				Segment->StartRadius = Segment->StartRadius * 1.1;
			}
		}
		Segment->EndRadius = Segment->StartRadius * 0.8;
		if (i < Segments.Num() - 1)
		{
			Segments[i + 1]->StartRadius = Segment->EndRadius;
		}
	}
}

UBranchSegmentCPP* UBranchCPP::AddNewSegment(FRotator Direction)
{
	CleanSegments();
	if (Segments.Num() >= MaxNSegments)
	{
		return nullptr;
	}
	UBranchSegmentCPP* NewSegment = NewObject<UBranchSegmentCPP>(this, UBranchSegmentCPP::StaticClass(), FName(FString::Printf(TEXT("Segment %d"), Segments.Num())));
	NewSegment->RegisterComponent();
	FVector NewLocation = FVector();
	NewSegment->Length = MaxSegmentLength / 2;
	if (Segments.Num() > 0)
	{
		UBranchSegmentCPP* AttachedToSegment = Segments.Last();
		NewSegment->StartRadius = AttachedToSegment->EndRadius;
		NewSegment->EndRadius = NewSegment->StartRadius * 0.8;
		NewSegment->AttachToComponent(AttachedToSegment, FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewLocation = AttachedToSegment->Length * FVector(0, 0, 1);
	}
	else
	{
		BaseSegment = NewSegment;
		NewSegment->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Roll = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	NewSegment->AddLocalRotation(FQuat::MakeFromRotator(Direction + RandomVariance));
	NewSegment->SetRelativeLocation(NewLocation);
	Nub->AttachToComponent(NewSegment, FAttachmentTransformRules::SnapToTargetIncludingScale);
	Nub->SetRelativeLocation(NewSegment->Length * FVector(0, 0, 1));
	Segments.Add(NewSegment);
	return NewSegment;
}