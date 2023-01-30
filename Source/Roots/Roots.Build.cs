// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Roots : ModuleRules
{
	public Roots(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "GeometryFramework", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
