// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NutrientPocketCPP.generated.h"

UCLASS()
class ROOTS_API ANutrientPocketCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANutrientPocketCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		float DrainNutrients(float Rate);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float StartingNutrients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RemainingNutrients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxNutrients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;

	UFUNCTION()
		void SetNewScale();
};
