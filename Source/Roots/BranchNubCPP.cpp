// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchNubCPP.h"

// Sets default values
UBranchNubCPP::UBranchNubCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
	StartRadius = 10;
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

