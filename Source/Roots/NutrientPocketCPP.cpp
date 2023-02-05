// Fill out your copyright notice in the Description page of Project Settings.


#include "NutrientPocketCPP.h"
#include <Roots/RootsDeveloperSettings.h>

// Sets default values
ANutrientPocketCPP::ANutrientPocketCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	StaticMesh->SetStaticMesh(DevSettings->NutrientsMeshes[FMath::RandRange(0, DevSettings->NutrientsMeshes.Num() - 1)].LoadSynchronous());
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Nutrients
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block); // Nutrients

	MaxNutrients = 1200;
	StartingNutrients = FMath::FRandRange(MaxNutrients / 2, MaxNutrients);
	RemainingNutrients = StartingNutrients;
}

// Called when the game starts or when spawned
void ANutrientPocketCPP::BeginPlay()
{
	Super::BeginPlay();
	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	StaticMesh->SetStaticMesh(DevSettings->NutrientsMeshes[FMath::RandRange(0, DevSettings->NutrientsMeshes.Num() - 1)].LoadSynchronous());
	SetNewScale();
	
}

// Called every frame
void ANutrientPocketCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ANutrientPocketCPP::DrainNutrients(float Rate)
{
	float Result = FMath::Clamp(Rate, 0, RemainingNutrients);
	RemainingNutrients -= Result;
	SetNewScale();
	return Result;
}

void ANutrientPocketCPP::SetNewScale()
{
	float NewScale = (RemainingNutrients + StartingNutrients) / (MaxNutrients * 2);
	StaticMesh->SetRelativeScale3D(FVector(NewScale));
}

