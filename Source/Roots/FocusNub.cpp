// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusNub.h"

// Sets default values
AFocusNub::AFocusNub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
}

// Called when the game starts or when spawned
void AFocusNub::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFocusNub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

