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
	CleanSegments();
	for (int i = 1; i < Segments.Num(); i++)
	{
		UBranchSegmentCPP* Segment = Segments[i];
		UBranchSegmentCPP* AttachedToSegment = Segments[i - 1];
		Segment->StartRadius = AttachedToSegment->EndRadius;
		Segment->EndRadius = Segment->StartRadius * 0.8;
		Segment->AttachToComponent(AttachedToSegment, FAttachmentTransformRules::KeepWorldTransform);
		Segment->SetRelativeLocation(AttachedToSegment->Length * FVector(0, 0, 1));
	}
	if (IsValid(Nub))
	{
		Nub->AttachToComponent(Segments.Last(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		Nub->SetRelativeLocation((Segments.Last()->Length + 10) * FVector(0, 0, 1));
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
	if (!IsValid(Nub) || Segments.IsEmpty())
	{
		return GetComponentRotation();
	}
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Roll = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	return  UKismetMathLibrary::FindLookAtRotation(Segments.Last()->GetEndLocation(), Nub->GetComponentLocation()) - FRotator(90, 0, 0) + RandomVariance;
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
	return FResourceSet(NSegmentsThatCanGrow, NSegmentsThatCanGrow);
}

void UBranchCPP::GrowSelf()
{
	for (int i = 0; i < Segments.Num(); i++)
	{
		UBranchSegmentCPP* Segment = Segments[i];
		UBranchSegmentCPP* PreviousSegment = i == 0 ? nullptr : Segments[i - 1];
		FVector GrowthOffset = FVector(0, 0, 0);
		FVector InitialEndLocation = Segment->GetEndLocation();
		if (Segment->GetSegmentLength() < MaxSegmentLength)
		{
			float NewLength = FMath::Clamp(Segment->Length * 1.1f, 0, MaxSegmentLength);
			Segment->Length = NewLength;
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
		if (i == Segments.Num() - 1 && IsValid(Nub))
		{
			Nub->AddWorldOffset(GrowthOffsetAtEnd);
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
	UBranchSegmentCPP* NewSegment = CreateDefaultSubobject<UBranchSegmentCPP>(FName(FString::Printf(TEXT("Segment%d"), Segments.Num())));
	NewSegment->RegisterComponent();
	NewSegment->Length = MaxSegmentLength / 2;
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Roll = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	NewSegment->AddLocalRotation(FQuat::MakeFromRotator(Direction + RandomVariance));
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
	UBranchSegmentCPP* NewSegment = NewObject<UBranchSegmentCPP>(this, UBranchSegmentCPP::StaticClass(), FName(FString::Printf(TEXT("Segment%d"), Segments.Num())));
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
	if (IsValid(Nub))
	{
		Nub->AttachToComponent(NewSegment, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Nub->SetRelativeLocation((NewSegment->Length + 10) * FVector(0, 0, 1));;
		if (Segments.Num() == MaxNSegments)
		{
			Nub->DestroyComponent();
		}
	}
	Segments.Add(NewSegment);
	return NewSegment;
}