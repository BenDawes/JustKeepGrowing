// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FResourceSet.h"
#include "BranchDirector.generated.h"

UCLASS(ClassGroup = "Roots System", meta = (BlueprintSpawnableComponent))
class ROOTS_API UBranchDirector : public USceneComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBranchDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;

	UFUNCTION()
		void TryAddWorldRotation(FRotator Rotator);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator BaselineRotator;

	void DisableDirectionPointer();
};
