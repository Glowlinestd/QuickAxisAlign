// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuickAxisAlign : ModuleRules
{
	public QuickAxisAlign(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"Slate",
				"SlateCore",
				"EditorFramework",
				"UnrealEd",
				"LevelEditor",
				"ToolMenus",
				"Projects",
				"PropertyEditor",
			}
		);
	}
}
