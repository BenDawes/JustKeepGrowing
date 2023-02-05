// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchDirector.h"
#include "RootsGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "RootsDeveloperSettings.h"

// Sets default values
UBranchDirector::UBranchDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	StaticMesh->SetStaticMesh(DevSettings->NubMeshPath.LoadSynchronous());
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Selectable
}

void UBranchDirector::OnRegister()
{
	Super::OnRegister();
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	StaticMesh->SetStaticMesh(DevSettings->DirectionPointerMeshPath.LoadSynchronous());
	StaticMesh->RegisterComponent();
	StaticMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

}

void UBranchDirector::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (IsValid(StaticMesh))
	{
		StaticMesh->DestroyComponent();
	}
}

// Called when the game starts or when spawned
void UBranchDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UBranchDirector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBranchDirector::TryAddWorldRotation(FRotator Rotator)
{
	FRotator CurrentRotation = GetComponentRotation();
	AddWorldRotation(Rotator);
	FRotator NewRotation = GetComponentRotation();
	if (UBranchSegmentCPP* Segment = Cast<UBranchSegmentCPP>(GetAttachParent()))
	{
		BaselineRotator = Segment->SegmentDirection;
	}
	if (UKismetMathLibrary::DegAcos(FVector::DotProduct(BaselineRotator.RotateVector(FVector::UpVector), NewRotation.RotateVector(FVector::UpVector))) > 35.f)
	{
		SetWorldRotation(CurrentRotation);
	}
	else
	{
		if (UBranchSegmentCPP* Segment = Cast<UBranchSegmentCPP>(GetAttachParent()))
		{
			Segment->SetGrowDirection(NewRotation);
		}
	}
}
