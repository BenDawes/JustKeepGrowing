// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BranchCPP.h"
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
	virtual void PostInitProperties() override;

	float CachedRadius;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBranchCPP* RootBranch;

	UFUNCTION(BlueprintCallable)
	void Grow(float NutrientsIn);

	UFUNCTION(BlueprintCallable)
		float GatherWater();

	UFUNCTION(BlueprintCallable)
		float GatherNutrients();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Radius;
};
