// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchSegmentCPP.h"
#include "BranchNubCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "BranchCPP.h"


// Sets default values
UBranchSegmentCPP::UBranchSegmentCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	StartRadius = 50;
	EndRadius = 40;
	Length = 150;
}



void UBranchSegmentCPP::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBranchSegmentCPP::OnRegister()
{
	Super::OnRegister();
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

}

void UBranchSegmentCPP::GenerateConnectionPoints()
{
	// TODO Calculate N supported branches (N = 1 for radius X, 2 for radius Y)
	// If ConnectedBranches.Num >= N, set to none
	// If diff > 0, generate random points along edge, 5 total, find 1 that is > X distance from all existing points
	// Random point along edge =Random theta, random offset between 0.25 and 0.75
}

FVector UBranchSegmentCPP::GetSegmentVector()
{
	FRotator Rot = GetComponentRotation();
	return Rot.RotateVector(FVector(1, 0, 0)) * Length;
}

void UBranchSegmentCPP::AddBranchAt(FVector ConnectionPoint)
{
	FTransform Transform = FTransform();
	FVector SpawnLocation = UKismetMathLibrary::ProjectVectorOnToVector(ConnectionPoint, GetSegmentVector());
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, ConnectionPoint) + FRotator(0, -90,  0);
	Transform.SetLocation(SpawnLocation);

	UBranchCPP* NewBranch = NewObject<UBranchCPP>(this, UBranchCPP::StaticClass(), FName(FString::Printf(TEXT("Branch %d"), ConnectedBranches.Num())));
	ConnectedBranches.Add(NewBranch);
	NewBranch->RegisterComponent();
	NewBranch->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	NewBranch->SetRelativeLocation(SpawnLocation);
	NewBranch->SetRelativeRotation(SpawnRotation);

	GenerateConnectionPoints();
}
