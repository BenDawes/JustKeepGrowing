// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchNubCPP.h"
#include "RootsDeveloperSettings.h"

// Sets default values
UBranchNubCPP::UBranchNubCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
	StartRadius = 10;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	StaticMesh->SetStaticMesh(DevSettings->NubMeshPath.LoadSynchronous());
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Selectable
	LargeScale = FVector(0.7f, 0.7f, 0.7f);
	SmallScale = FVector(0.4f, 0.4f, 0.4f);
}

void UBranchNubCPP::OnRegister()
{
	Super::OnRegister();
	StaticMesh->RegisterComponent();
	StaticMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SetWantsToGrow(false);
	SetShow(false);

}

void UBranchNubCPP::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (IsValid(StaticMesh))
	{
		StaticMesh->DestroyComponent();
	}
}

// Called when the game starts or when spawned
void UBranchNubCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UBranchNubCPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FResourceSet UBranchNubCPP::GetGrowthCost()
{
	return FResourceSet(10, 10);
}

void UBranchNubCPP::SetWantsToGrow(bool NewWantsToGrow)
{
	bWantsToGrow = NewWantsToGrow;
	StaticMesh->SetWorldScale3D(bWantsToGrow ? LargeScale : SmallScale);
}

void UBranchNubCPP::SetShow(bool bShow)
{
	// StaticMesh->SetVisibility(bShow);
	if (bShow)
	{
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Selectable
	}
	else
	{
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); // not Selectable
	}
}

