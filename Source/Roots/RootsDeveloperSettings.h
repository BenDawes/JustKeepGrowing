// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RootsGameState.h"
#include "RootsDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Dev Settings")) // Give it a better looking name in UI
class ROOTS_API URootsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	/* Soft path will be converted to content reference before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
		TSoftObjectPtr<UStaticMesh> NubMeshPath;
	/* Soft path will be converted to content reference before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
		TSoftObjectPtr<UStaticMesh> DirectionPointerMeshPath;
	/* Soft path will be converted to content reference before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
		TArray<TSoftObjectPtr<UStaticMesh>> NutrientsMeshes;

	/* Soft path will be converted to content reference before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
		TSubclassOf<ARootsGameState> GameStateClass = ARootsGameState::StaticClass();

	URootsDeveloperSettings();
};