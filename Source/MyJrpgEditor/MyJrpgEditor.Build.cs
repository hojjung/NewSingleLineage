// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class MyJrpgEditor : ModuleRules
{
	public MyJrpgEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			"MyJrpgEditor/Public",
		});

		PrivateIncludePaths.AddRange(new string[] {
            "MyJrpgEditor/Private",
		});
			
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Json",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"UnrealEd",
				"DataTableEditor",
				"AssetTools",
				"InputCore",
				"EditorStyle",
                "MyJrpg"
            }
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Json",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"UnrealEd",
				"DataTableEditor",
				"AssetTools",
				"InputCore",
				"EditorStyle",
                "MyJrpg"
            }
		);
		

		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}
