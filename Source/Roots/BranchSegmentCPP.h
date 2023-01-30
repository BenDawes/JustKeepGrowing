// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "BranchSegmentCPP.generated.h"

class UBranchCPP;
class UBranchNubCPP;

UCLASS()
class ROOTS_API UBranchSegmentCPP : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBranchSegmentCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		UBranchCPP* ParentBranch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EndRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator Direction;

	UFUNCTION(BlueprintCallable)
		void GenerateConnectionPoints();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector> ConnectionPoints;

	UFUNCTION(BlueprintCallable)
		void AddBranchAt(FVector ConnectionPointWorld);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UBranchCPP*> ConnectedBranches;

	UFUNCTION(BlueprintCallable)
		FVector GetSegmentVector();

};
