// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class NPCPaletteEditor : ModuleRules
{
	public NPCPaletteEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"NPCPalette"
			}
		);
		PrivateDependencyModuleNames.AddRange( 
			new string[] { 
				"Core", 
				"CoreUObject",
				"Engine", 
				"InputCore",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"ContentBrowser",
				"CollectionManager",
				"LevelEditor",
				"AssetTools",
				"EditorStyle",
				"NPCPalette",
				"EditorWidgets",
				"PropertyEditor",
				"UMG"
			} 
		);
	}
}
