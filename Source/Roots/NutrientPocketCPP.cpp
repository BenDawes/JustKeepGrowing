// Fill out your copyright notice in the Description page of Project Settings.


#include "NutrientPocketCPP.h"

// Sets default values
ANutrientPocketCPP::ANutrientPocketCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	StartingNutrients = 500;
	RemainingNutrients = 500;
}

// Called when the game starts or when spawned
void ANutrientPocketCPP::BeginPlay()
{
	Super::BeginPlay();
	
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
	float NewScale = (RemainingNutrients + StartingNutrients) / (StartingNutrients * 2);
	StaticMesh->SetRelativeScale3D(FVector(NewScale));
	return Result;
}

