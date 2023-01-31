// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BranchNubCPP.h"
#include "BranchSegmentCPP.h"
#include "FResourceSet.h"
#include "BranchCPP.generated.h"

UCLASS(ClassGroup = "Roots System", meta = (BlueprintSpawnableComponent))
class ROOTS_API UBranchCPP : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBranchCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void OnRegister() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UBranchSegmentCPP*> Segments;

	UFUNCTION(BlueprintCallable)
		void GenerateConnectionPoints();

	UFUNCTION(BlueprintCallable)
		UBranchSegmentCPP* AddNewSegment(FRotator Direction);

	UFUNCTION(BlueprintCallable)
		UBranchSegmentCPP* AddNewSegmentConstructor(FRotator Direction);

	UFUNCTION(BlueprintCallable)
		void CleanSegments();

	UFUNCTION(BlueprintCallable)
		FRotator GetGrowDirection();

	UFUNCTION(BlueprintCallable)
		FResourceSet GatherResources();

	UFUNCTION(BlueprintCallable)
		float GetLength();
	UFUNCTION(BlueprintCallable)
		TArray<UBranchSegmentCPP*> GetAllSubSegments();

	UFUNCTION(BlueprintCallable)
		FResourceSet Grow(FResourceSet InputResources);

	UFUNCTION(BlueprintCallable)
		bool CanSupportNewSegmentAtEnd();

	UFUNCTION(BlueprintCallable)
		FResourceSet GetResourcesNeededForNewSegment();

	UFUNCTION(BlueprintCallable)
		bool CanGrowSelf();
	UFUNCTION(BlueprintCallable)
		FResourceSet GetGrowthCost();
	UFUNCTION(BlueprintCallable)
		void GrowSelf();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxSegmentLength = 200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MaxNSegments = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxVarianceAngle = 50.f;

	UFUNCTION(BlueprintCallable)
		UBranchSegmentCPP* GetRandomSubSegment();
};
