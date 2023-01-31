// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchSegmentCPP.h"
#include "BranchNubCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "BranchCPP.h"
#include <Kismet/GameplayStatics.h>
#include "ResourceSetFunctions.h"
#include "RootsDeveloperSettings.h"


// Sets default values
UBranchSegmentCPP::UBranchSegmentCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	StartRadius = 20;
	EndRadius = 16;
	Length = 120;

	PointerMesh = CreateDefaultSubobject<UStaticMeshComponent>("PointerMesh");
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	PointerMesh->SetStaticMesh(DevSettings->NubMeshPath.LoadSynchronous());
}



void UBranchSegmentCPP::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBranchSegmentCPP::OnRegister()
{
	Super::OnRegister();
	GenerateConnectionPoints();
	PointerMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PointerMesh->SetVisibility(false);
}

void UBranchSegmentCPP::ShowPointer()
{
	if (IsValid(PointerMesh))
	{
		PointerMesh->SetVisibility(true);
		PointerMesh->SetRelativeLocation(FVector(0,0,Length + 10));
		PointerMesh->SetWorldRotation(GetComponentRotation());
	}
}

void UBranchSegmentCPP::HidePointer()
{
	if (IsValid(PointerMesh))
	{
		PointerMesh->SetVisibility(false);
	}

}

// Called when the game starts or when spawned
void UBranchSegmentCPP::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UBranchSegmentCPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ShowPointer();
}

void UBranchSegmentCPP::SetLength(float NewLength)
{
	Length = NewLength;
}

void UBranchSegmentCPP::GenerateConnectionPoints()
{
	ConnectionPoints = TArray<FVector>();
	if (StartRadius < 30 || ConnectedBranches.Num() == 2)
	{
		return;
	}

	FVector CandidatePoint = GetRandomFreePointOnEdge();
	if (!CandidatePoint.IsNearlyZero())
	{
		ConnectionPoints.Add(CandidatePoint);
	}

	if (StartRadius < 40 || ConnectedBranches.Num() == 2)
	{
		return;
	}
	CandidatePoint = GetRandomFreePointOnEdge();
	if (!CandidatePoint.IsNearlyZero())
	{
		ConnectionPoints.Add(CandidatePoint);
	}
	for (UBranchCPP* Branch : ConnectedBranches)
	{
		Branch->GenerateConnectionPoints();
	}
}

FVector UBranchSegmentCPP::GetEndLocation()
{
	FRotator Rot = GetComponentRotation();
	return GetComponentLocation() + Rot.RotateVector(FVector(0, 0, 1)) * Length; 
}

FVector UBranchSegmentCPP::GetRandomFreePointOnEdge()
{
	CleanBranches();
	CleanNubs();
	int MaxAttempts = 8;
	float DistanceThreshold = (StartRadius * 3.f) * (StartRadius * 3.f);
	for (int i = 0; i < MaxAttempts; i++)
	{
		float Fraction = .25f + (FMath::FRand() / 2.f);
		float Theta = FMath::Rand() * 360.f;
		float RadiusAtOffset = EndRadius + ((StartRadius - EndRadius) * Fraction);
		FVector SegmentVector = GetEndLocation() - GetComponentLocation();
		FVector RelativeVectorToCenter = SegmentVector * Fraction;
		FVector RadiusVector = UKismetMathLibrary::RotatorFromAxisAndAngle(SegmentVector, Theta).RotateVector(RelativeVectorToCenter.ToOrientationQuat().GetRightVector().GetSafeNormal() * RadiusAtOffset);
		FVector Point = GetComponentLocation() + RelativeVectorToCenter + RadiusVector;
		if (ConnectionPoints.ContainsByPredicate([Point, DistanceThreshold](FVector OtherPoint) -> bool { return FVector::DistSquared(OtherPoint, Point) < DistanceThreshold; }))
		{
			continue;
		}
		if (ConnectedBranches.ContainsByPredicate([Point, DistanceThreshold](UBranchCPP* Branch) -> bool { return FVector::DistSquared(Branch->GetComponentLocation(), Point) < DistanceThreshold; }))
		{
			continue;
		}
		if (NubsWantingToGrow.ContainsByPredicate([Point, DistanceThreshold](UBranchNubCPP* Nub) -> bool { return FVector::DistSquared(Nub->GetComponentLocation(), Point) < DistanceThreshold; }))
		{
			continue;
		}
		return Point;

	}
	return FVector::ZeroVector;
}

FResourceSet UBranchSegmentCPP::GatherResources()
{
	if (!EnsureEarth())
	{
		return FResourceSet();
	}
	float AccumulatedWater = 0.f;
	float AccumulatedNutrients = 0.f;
	AccumulatedWater += Earth->GetWater(GetComponentLocation(), GetEndLocation(), StartRadius, SegmentGatherRate);
	AccumulatedNutrients += Earth->DrainNutrients(GetComponentLocation(), GetEndLocation(), StartRadius, SegmentGatherRate);
	
	for (UBranchCPP* ConnectedBranch : ConnectedBranches)
	{
		FResourceSet GatheredFromBranch = ConnectedBranch->GatherResources();
		AccumulatedWater += GatheredFromBranch.Water;
		AccumulatedNutrients += GatheredFromBranch.Nutrients;
	}
	return FResourceSet(AccumulatedWater, AccumulatedNutrients);
}

bool UBranchSegmentCPP::EnsureEarth()
{
	if (IsValid(Earth))
	{
		return true;
	}
	if (UWorld* World = GetWorld())
	{
		Earth = Cast<AEarthCPP>(UGameplayStatics::GetActorOfClass(World, AEarthCPP::StaticClass()));
	}
	return IsValid(Earth);
}

void UBranchSegmentCPP::CleanBranches()
{
	TArray<UBranchCPP*> NewBranches;
	for (UBranchCPP* Branch : ConnectedBranches)
	{
		if (IsValid(Branch))
		{
			NewBranches.Add(Branch);
		}
	}
	ConnectedBranches = NewBranches;
}

void UBranchSegmentCPP::CleanNubs()
{
	TArray<UBranchNubCPP*> NewNubs;
	for (UBranchNubCPP* GrowthNub : NubsWantingToGrow)
	{
		if (IsValid(GrowthNub))
		{
			NewNubs.Add(GrowthNub);
		}
	}
	NubsWantingToGrow = NewNubs;
}

float UBranchSegmentCPP::GetSubLength()
{
	float AccumulatedLength = 0;
	for (UBranchCPP* ConnectedBranch : ConnectedBranches)
	{
		AccumulatedLength += ConnectedBranch->GetLength();
	}
	return AccumulatedLength;
}

float UBranchSegmentCPP::GetSegmentLength()
{
	return Length;
}

FResourceSet UBranchSegmentCPP::Grow(FResourceSet InputResources)
{
	FResourceSet ResultSet = InputResources;
	for (UBranchCPP* Branch : ConnectedBranches)
	{
		ResultSet = Branch->Grow(ResultSet);
	}
	for (UBranchNubCPP* GrowthNub : NubsWantingToGrow)
	{
		FResourceSet GrowthCost = GrowthNub->GetGrowthCost();
		if (UResourceSetFunctions::HasResources(ResultSet, GrowthCost))
		{
			ResultSet = UResourceSetFunctions::DrainResources(ResultSet, GrowthCost);
			AddBranchAt(GrowthNub->GetRelativeLocation());
		}
		GrowthNub->DestroyComponent();
	}
	GenerateConnectionPoints();
	return ResultSet;
}

void UBranchSegmentCPP::AddBranchAt(FVector ConnectionPoint)
{
	FVector SpawnLocation = UKismetMathLibrary::FindClosestPointOnLine(ConnectionPoint, GetComponentLocation(), GetEndLocation() - GetComponentLocation());
	FVector VectorFromSpawnPointToConnectionPoint = (ConnectionPoint - SpawnLocation);
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Roll = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, ConnectionPoint) + RandomVariance;

	UBranchCPP* NewBranch = NewObject<UBranchCPP>(this, UBranchCPP::StaticClass(), FName(FString::Printf(TEXT("Branch%d"), ConnectedBranches.Num())));
	ConnectedBranches.Add(NewBranch);
	NewBranch->RegisterComponent();
	NewBranch->SetWorldLocation(SpawnLocation);
	NewBranch->SetWorldRotation(SpawnRotation);
	NewBranch->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	NewBranch->AddNewSegment(SpawnRotation);
	GenerateConnectionPoints();
}

void UBranchSegmentCPP::AddBranch()
{
	if (ConnectionPoints.IsEmpty())
	{
		return;
	}
	AddBranchAt(ConnectionPoints[FMath::RandRange(0, ConnectionPoints.Num() - 1)]);
}

