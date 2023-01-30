// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchCPP.h"
#include "Kismet/KismetMathLibrary.h"

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

void UBranchCPP::AddNewSegment(FRotator Direction)
{
	CleanSegments();
	UBranchSegmentCPP* NewSegment = NewObject<UBranchSegmentCPP>(this, UBranchSegmentCPP::StaticClass(), FName(FString::Printf(TEXT("Segment %d"), Segments.Num())));
	NewSegment->RegisterComponent();
	FVector NewLocation = FVector();
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
	NewSegment->AddLocalRotation(FQuat::MakeFromRotator(Direction));
	NewSegment->SetRelativeLocation(NewLocation);
	Nub->AttachToComponent(NewSegment, FAttachmentTransformRules::SnapToTargetIncludingScale);
	Nub->SetRelativeLocation(NewSegment->Length * FVector(0, 0, 1));
	Segments.Add(NewSegment);
}