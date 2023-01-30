// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FResourceSet.h"
#include "ResourceContainer.generated.h"


UCLASS()
class ROOTS_API UResourceContainer : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UResourceContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FResourceSet Resources;

	UFUNCTION(BlueprintCallable)
		float AddWater(float IncomingWater);

	UFUNCTION(BlueprintCallable)
		float AddNutrients(float IncomingNutrients);

	UFUNCTION(BlueprintCallable)
		FResourceSet AddResources(FResourceSet IncomingResources);

	UFUNCTION(BlueprintCallable)
		bool DrainWater(float DesiredWater);

	UFUNCTION(BlueprintCallable)
		bool DrainNutrients(float DesiredNutrients);

	UFUNCTION(BlueprintCallable)
		bool HasWater(float CheckAmount);

	UFUNCTION(BlueprintCallable)
		bool HasNutrients(float CheckAmount);

	UFUNCTION(BlueprintCallable)
		bool HasResources(FResourceSet CheckAmount);

	UFUNCTION(BlueprintCallable)
		FResourceSet DrainResources(FResourceSet DesiredAmounts);

	UFUNCTION(BlueprintCallable)
		void SetResources(FResourceSet NewResources);
};
