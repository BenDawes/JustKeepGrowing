// Copyright Epic Games, Inc. All Rights Reserved.

#include "RootsGameMode.h"
#include "RootsGameState.h"
#include "RootsCharacter.h"
#include "RootsDeveloperSettings.h"
#include "UObject/ConstructorHelpers.h"

ARootsGameMode::ARootsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	const URootsDeveloperSettings* DevSettings = GetDefault<URootsDeveloperSettings>(); // Access via CDO
	GameStateClass = DevSettings->GameStateClass != nullptr ? DevSettings->GameStateClass : ARootsGameState::StaticClass();
}
