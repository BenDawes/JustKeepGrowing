// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "EarthCPP.h"
#include "BranchDirector.h"
#include "BranchSegmentCPP.generated.h"
class UBranchCPP;
class UBranchNubCPP;

UCLASS(ClassGroup = "Roots System", meta = (BlueprintSpawnableComponent))
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
	UFUNCTION()
		void AdjustCollider();
	UPROPERTY()
		bool bPointerMeshDisabled = false;

	UFUNCTION()
		void OnSelected();

	UFUNCTION()
		void OnUnselected();

	UFUNCTION()
		void DisableDirectionPointer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EndRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Length;

	UFUNCTION(BlueprintCallable)
		void SetLength(float NewLength);

	UFUNCTION(BlueprintCallable)
		void GenerateConnectionPoints();
	UFUNCTION(BlueprintCallable)
		TArray<FVector> GetConnectionVectors();


	UFUNCTION(BlueprintCallable)
		void AddBranchAt(FVector ConnectionPointWorld);

	UFUNCTION(BlueprintCallable)
		void AddBranch();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UBranchCPP*> ConnectedBranches;

	UFUNCTION(BlueprintCallable)
		FVector GetEndLocation();

	UFUNCTION(BlueprintCallable)
		FVector GetRandomFreePointOnEdge();

	UFUNCTION(BlueprintCallable)
		FResourceSet GatherResources();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AEarthCPP* Earth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SegmentGatherRate = 10;

	UFUNCTION(BlueprintCallable)
		bool EnsureEarth();
	UFUNCTION(BlueprintCallable)
		void CleanBranches();
	UFUNCTION(BlueprintCallable)
		void CleanNubs();
	UFUNCTION(BlueprintCallable)
		float GetSubLength();

	UFUNCTION(BlueprintCallable)
		float GetSegmentLength();

	UFUNCTION(BlueprintCallable)
		FResourceSet Grow(FResourceSet InputResources);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxVarianceAngle = 15.f;

	UPROPERTY()
		FRotator SegmentDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* PointerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UBranchDirector* BranchDirector;
	UFUNCTION(BlueprintCallable)
		void ShowPointer();
	UFUNCTION(BlueprintCallable)
		void HidePointer();

	bool bIsSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCapsuleComponent* CapsuleComponent;

	UFUNCTION()
		void DebugCall();

	UPROPERTY(BlueprintReadOnly)
		FRotator GrowDirectionWorld;

	UFUNCTION(BlueprintCallable)
		void CalculateNewDirection(APlayerController* PC, FVector2D ScreenPosition, FVector CharacterLocation, FRotator CharacterViewRotation);

	void ClampGrowDirection();

	UFUNCTION()
		void SetGrowDirection(FRotator NewDirection);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ClampAngleTolerance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UBranchNubCPP*> ConnectionPointNubs;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator RotationOffset;

	UFUNCTION()
		void OnConfigurationChanged();
	UFUNCTION()
		void AdjustPointerMesh();

};

