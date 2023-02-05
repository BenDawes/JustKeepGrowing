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
	ClampAngleTolerance = 35;

	PointerMesh = CreateDefaultSubobject<UStaticMeshComponent>("PointerMesh");
	BranchDirector = CreateDefaultSubobject<UBranchDirector>("BranchDirector");

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->InitCapsuleSize(StartRadius - 5, (Length / 2) - 5);
	RotationOffset = FRotator(90, 0, 0);
};

void UBranchSegmentCPP::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBranchSegmentCPP::OnRegister()
{
	Super::OnRegister();
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	PointerMesh->SetStaticMesh(DevSettings->DirectionPointerMeshPath.LoadSynchronous());
	PointerMesh->RegisterComponent();
	PointerMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PointerMesh->SetVisibility(false);
	CapsuleComponent->RegisterComponent();
	CapsuleComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	BranchDirector->RegisterComponent();
	BranchDirector->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OnConfigurationChanged();
	// PointerMesh->DestroyComponent();
}

void UBranchSegmentCPP::ShowPointer()
{
	if (IsValid(PointerMesh) && !bPointerMeshDisabled)
	{
		PointerMesh->SetVisibility(true);
		AdjustPointerMesh();
	}
}

void UBranchSegmentCPP::HidePointer()
{
	if (IsValid(PointerMesh))
	{
		PointerMesh->SetVisibility(false);
	}

}

void UBranchSegmentCPP::DebugCall()
{
	UE_LOG(LogTemp, Warning, TEXT("Debug hit!"));
}

void UBranchSegmentCPP::CalculateNewDirection(APlayerController* PC, FVector2D ScreenPosition, FVector CharacterLocation, FRotator CharacterViewRotation)
{
	/*
	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(PC, ScreenPosition, WorldOrigin, WorldDirection) == true)
	{
		FVector MeshLocation = PointerMesh->GetComponentLocation();
		FVector TargetPoint = UKismetMathLibrary::FindClosestPointOnLine(MeshLocation, WorldOrigin, WorldDirection);
		FQuat UnAdjustedRotator = (TargetPoint - MeshLocation).ToOrientationQuat();
		// Our actual growth direction is a rotator that applies to 0,0,1 to give the vector direction we're growing in
		//DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + (UnAdjustedRotator.RotateVector(FVector::ForwardVector) * 20), FColor::Blue, false, 3.f, 10U);
		FVector NewPointVector = UnAdjustedRotator.RotateVector(FVector::DownVector);
		//DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + (NewPointVector * 20), FColor::Green, false, 3.f, 10U);

		GrowDirectionWorld = NewPointVector.ToOrientationRotator(); // -Adjustment;
		if ((TargetPoint - MeshLocation).Z < 0)
		{
			GrowDirectionWorld += FRotator(180,0,0);
		}
		//DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + (GrowDirectionWorld.RotateVector(FVector::UpVector) * 20), FColor::Green, false, 3.f, 10U);

		// GrowDirectionWorld = (-GrowDirectionWorld.Vector()).ToOrientationRotator();
		//GrowDirectionWorld += UKismetMathLibrary::RotatorFromAxisAndAngle(WorldDirection, -90);
	}
	*/

	//ClampGrowDirection();
	//PointerMesh->SetWorldRotation(GrowDirectionWorld);
}

void UBranchSegmentCPP::ClampGrowDirection()
{
	FRotator ClampedGrowDirectionWorld = GrowDirectionWorld.Vector().ToOrientationRotator().Clamp(); // Force Roll 0
	FVector MeshLocation = PointerMesh->GetComponentLocation();

	FRotator SegmentDirectionAsRotator = (GetEndLocation() - GetComponentLocation()).ToOrientationRotator().Clamp(); // Force Roll 0
	DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + (GrowDirectionWorld.RotateVector(FVector::UpVector) * 20), FColor::Green, false, 3.f, 10U);
	DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + (ClampedGrowDirectionWorld.RotateVector(FVector::UpVector) * 20), FColor::Blue, false, 3.f, 10U);
	DrawDebugLine(GetWorld(), MeshLocation, MeshLocation + (SegmentDirectionAsRotator.RotateVector(FVector::UpVector) * 20), FColor::Red, false, 3.f, 10U);
	FRotator MinValues = SegmentDirectionAsRotator - FRotator(ClampAngleTolerance, ClampAngleTolerance, ClampAngleTolerance);
	FRotator MaxValues = SegmentDirectionAsRotator + FRotator(ClampAngleTolerance, ClampAngleTolerance, ClampAngleTolerance);
	/*DrawDebugLine
	(
		GetWorld(),
		GetEndLocation(),
		GetEndLocation() + (ClampedGrowDirectionWorld.Vector() * 50),
		FColor::Red,
		false,
		2.f,
		10U
	);
	DrawDebugLine
	(
		GetWorld(),
		GetEndLocation(),
		GetEndLocation() + (SegmentDirectionAsRotator.Vector() * 50),
		FColor::Red,
		false,
		2.f,
		10U
	);*/
	auto ClampValue = [](float Val, float Min, float Max) -> float {
		float ResVal = Val;
		if (Min < 0 && ResVal > Max && ResVal < 360 + Min) {
			ResVal = (ResVal - Max) > (360 + Min - ResVal) ? Min : Max;
		}
		if (Max > 360 && ResVal < Min && ResVal > Max - 360) {
			ResVal = (Min - ResVal) > (ResVal - (Max - 360)) ? Max : Min;
		}
		if (Min > 0 && ResVal < Min) {
			ResVal = Min;
		}
		if (Max < 360 && ResVal > Max) {
			ResVal = Max;
		}
		return ResVal;
	};

	GrowDirectionWorld = FRotator(
		ClampValue(ClampedGrowDirectionWorld.Pitch, MinValues.Pitch, MaxValues.Pitch),
		ClampValue(ClampedGrowDirectionWorld.Yaw, MinValues.Yaw, MaxValues.Yaw),
		ClampValue(ClampedGrowDirectionWorld.Roll, MinValues.Roll, MaxValues.Roll)
	).Clamp();
}

void UBranchSegmentCPP::SetGrowDirection(FRotator NewDirection)
{
	GrowDirectionWorld = NewDirection;
}

void UBranchSegmentCPP::OnConfigurationChanged()
{
	AdjustCollider();
	AdjustPointerMesh();
	GenerateConnectionPoints();
}

void UBranchSegmentCPP::AdjustPointerMesh()
{
	PointerMesh->SetRelativeLocation(FVector(0, 0, Length + 100));
	PointerMesh->SetWorldRotation(GrowDirectionWorld);
	BranchDirector->SetRelativeLocation(FVector(0, 0, Length + 10));
	BranchDirector->SetWorldRotation(GrowDirectionWorld);
}

// Called when the game starts or when spawned
void UBranchSegmentCPP::BeginPlay()
{
	Super::BeginPlay();
	ARootsGameState* const GameState = GetWorld()->GetGameState<ARootsGameState>();
	SegmentGatherRate = GameState->SegmentGatherRate;
}

// Called every frame
void UBranchSegmentCPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBranchSegmentCPP::AdjustCollider()
{
	CapsuleComponent->SetCapsuleRadius(StartRadius - 5);
	CapsuleComponent->SetCapsuleHalfHeight((Length / 2) - 5);
	CapsuleComponent->SetWorldLocation(FMath::Lerp(GetComponentLocation(), GetEndLocation(), 0.5));
	CapsuleComponent->SetWorldRotation(GetComponentRotation());
	//DrawDebugCapsule(GetWorld(), CapsuleComponent->GetComponentLocation(), CapsuleComponent->GetUnscaledCapsuleHalfHeight(), CapsuleComponent->GetUnscaledCapsuleRadius(), CapsuleComponent->GetComponentQuat(), FColor::Blue, false, 15.f, 10U);
}

void UBranchSegmentCPP::OnSelected()
{
	bIsSelected = true;
	ShowPointer();
	for (UBranchNubCPP* Nub : ConnectionPointNubs)
	{
		if (IsValid(Nub))
		{
			Nub->SetShow(true);
		}
	}
}

void UBranchSegmentCPP::OnUnselected()
{
	bIsSelected = false;
	HidePointer();
	for (UBranchNubCPP* Nub : ConnectionPointNubs)
	{
		if (IsValid(Nub))
		{
			Nub->SetShow(false);
		}
	}
}

void UBranchSegmentCPP::DisableDirectionPointer()
{
	bPointerMeshDisabled = true;
	PointerMesh->SetVisibility(false);
}

void UBranchSegmentCPP::SetLength(float NewLength)
{
	Length = NewLength;
	AdjustCollider();
}

void UBranchSegmentCPP::GenerateConnectionPoints()
{
	for (UBranchNubCPP* Nub : ConnectionPointNubs)
	{
		if (!IsValid(Nub))
		{
			continue;
		}
		Nub->DestroyComponent();
	}
	ConnectionPointNubs = TArray<UBranchNubCPP*>();
	TArray<FVector> NewConnectionPoints = GetConnectionVectors();

	for (FVector NubLocation : NewConnectionPoints)
	{
		UBranchNubCPP* NewNub = NewObject<UBranchNubCPP>(this, UBranchNubCPP::StaticClass(), FName(FString::Printf(TEXT("Nub%d"), ConnectionPointNubs.Num())));
		NewNub->RegisterComponent();
		NewNub->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewNub->SetWorldLocation(NubLocation);
		ConnectionPointNubs.Add(NewNub);
	}
}

TArray<FVector> UBranchSegmentCPP::GetConnectionVectors()
{
	TArray<FVector> NewConnectionPoints = TArray<FVector>();

	UBranchCPP* ParentBranch = Cast<UBranchCPP>(GetAttachParent());
	bool bIsFirstSegmentOnBranch = IsValid(ParentBranch) ? !ParentBranch->Segments.IsEmpty() && ParentBranch->Segments[0] == this : false;
	if (bIsFirstSegmentOnBranch || StartRadius < 20 || ConnectedBranches.Num() == 2)
	{
		return NewConnectionPoints;
	}

	FVector CandidatePoint = GetRandomFreePointOnEdge();
	if (!CandidatePoint.IsNearlyZero())
	{
		NewConnectionPoints.Add(CandidatePoint);
	}

	if (StartRadius < 30 || ConnectedBranches.Num() == 2)
	{
		return NewConnectionPoints;
	}
	CandidatePoint = GetRandomFreePointOnEdge();
	if (!CandidatePoint.IsNearlyZero())
	{
		NewConnectionPoints.Add(CandidatePoint);
	}
	return NewConnectionPoints;
}

FVector UBranchSegmentCPP::GetEndLocation()
{
	FVector EndLocation = GetComponentLocation() + SegmentDirection.RotateVector(FVector(0, 0, 1)) * Length;
	//DrawDebugSphere(GetWorld(), EndLocation, 10.f, 10, FColor::Red, false, 3.f, 10U);
	return EndLocation;
}

FVector UBranchSegmentCPP::GetRandomFreePointOnEdge()
{
	CleanBranches();
	CleanNubs();
	int MaxAttempts = 8;
	float DistanceThreshold = (StartRadius * 5.f) * (StartRadius * 5.f);
	for (int i = 0; i < MaxAttempts; i++)
	{
		float Fraction = .25f + (FMath::FRand() / 2.f);
		float Theta = FMath::FRand() * 360.f;
		float RadiusAtOffset = EndRadius + ((StartRadius - EndRadius) * Fraction);
		FVector SegmentVector = GetEndLocation() - GetComponentLocation();
		FVector RelativeVectorToCenter = SegmentVector * Fraction;
		FVector RadiusVector = UKismetMathLibrary::RotatorFromAxisAndAngle(SegmentVector, Theta).RotateVector(SegmentVector.ToOrientationQuat().GetRightVector().GetSafeNormal() * RadiusAtOffset);
		FVector Point = GetComponentLocation() + RelativeVectorToCenter + RadiusVector;
		if (ConnectedBranches.ContainsByPredicate([Point, DistanceThreshold](UBranchCPP* Branch) -> bool { return FVector::DistSquared(Branch->GetComponentLocation(), Point) < DistanceThreshold; }))
		{
			continue;
		}
		if (ConnectionPointNubs.ContainsByPredicate([Point, DistanceThreshold](UBranchNubCPP* Nub) -> bool { return FVector::DistSquared(Nub->GetComponentLocation(), Point) < DistanceThreshold; }))
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
	for (UBranchNubCPP* GrowthNub : ConnectionPointNubs)
	{
		if (IsValid(GrowthNub))
		{
			NewNubs.Add(GrowthNub);
		}
	}
	ConnectionPointNubs = NewNubs;
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
	for (UBranchNubCPP* GrowthNub : ConnectionPointNubs)
	{
		if (GrowthNub->bWantsToGrow)
		{
			FResourceSet GrowthCost = GrowthNub->GetGrowthCost();
			if (UResourceSetFunctions::HasResources(ResultSet, GrowthCost))
			{
				ResultSet = UResourceSetFunctions::DrainResources(ResultSet, GrowthCost);
				AddBranchAt(GrowthNub->GetComponentLocation());
				GrowthNub->DestroyComponent();
			}
		}
	}
	OnConfigurationChanged();
	return ResultSet;
}

void UBranchSegmentCPP::AddBranchAt(FVector ConnectionPoint)
{
	FVector SpawnLocation = UKismetMathLibrary::FindClosestPointOnLine (ConnectionPoint, GetComponentLocation(), GetEndLocation() - GetComponentLocation());
	FRotator RandomVariance;
	RandomVariance.Yaw = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	RandomVariance.Pitch = (FMath::FRand() * (MaxVarianceAngle * 2)) - MaxVarianceAngle;
	FRotator SpawnRotation = (SpawnLocation - ConnectionPoint).ToOrientationRotator() + FVector::UpVector.ToOrientationRotator() + RandomVariance;

	UBranchCPP* NewBranch = NewObject<UBranchCPP>(this, UBranchCPP::StaticClass(), FName(FString::Printf(TEXT("Branch%d"), ConnectedBranches.Num())));
	ConnectedBranches.Add(NewBranch);
	NewBranch->SetWorldLocation(SpawnLocation);
	NewBranch->SetWorldRotation(SpawnRotation);
	NewBranch->RegisterComponent();
	NewBranch->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	NewBranch->AddNewSegment(SpawnRotation);
}

void UBranchSegmentCPP::AddBranch()
{
	if (ConnectionPointNubs.IsEmpty())
	{
		return;
	}
	
	UBranchNubCPP* ChosenNub = ConnectionPointNubs[FMath::RandRange(0, ConnectionPointNubs.Num() - 1)];
	AddBranchAt(ChosenNub->GetComponentLocation());
	ChosenNub->DestroyComponent();
	GenerateConnectionPoints();
}

