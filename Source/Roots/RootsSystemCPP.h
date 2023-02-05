// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BranchCPP.h"
#include "ResourceContainer.h"
#include "RootsSystemCPP.generated.h"

UCLASS()
class ROOTS_API ARootsSystemCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARootsSystemCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	float CachedRadius;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UResourceContainer* StoredResources;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBranchCPP* RootBranch;

	UFUNCTION(BlueprintCallable)
	void Grow();

	UFUNCTION(BlueprintCallable)
		FResourceSet GatherResources();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet CostToGrowNewNub;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet CostToGrowSingleSegment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet CostToGrowNewSegment;
	UFUNCTION(BlueprintImplementableEvent)
		void OnGrowFinished();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet StartingResources;

	FResourceSet CapResources(FResourceSet InResources);

	
};
