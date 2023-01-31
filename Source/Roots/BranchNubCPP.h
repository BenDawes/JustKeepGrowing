// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FResourceSet.h"
#include "BranchNubCPP.generated.h"

UCLASS(ClassGroup = "Roots System", meta = (BlueprintSpawnableComponent))
class ROOTS_API UBranchNubCPP : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBranchNubCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float StartRadius;

	UFUNCTION(BlueprintCallable)
		FResourceSet GetGrowthCost();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;
};