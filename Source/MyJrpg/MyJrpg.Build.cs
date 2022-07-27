// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MyJrpg : ModuleRules
{
	public MyJrpg(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = true;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", 
			"InputCore", "AIModule","NavigationSystem",
			"CustomSlates","NPCPalette","BUITween","DialoguePlugin","DBTween","GameplayCameras",
			"MoviePlayer", "Http","Json", "JsonUtilities", "OnlineSubsystem", "OnlineSubsystemUtils",
			"PlayFabCommon", "PlayFabCpp" , "PlayFab"
		});// ,"RVOMovement","CustomSlates"

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate", "SlateCore",
			"CustomSlates","NPCPalette","BUITween","DialoguePlugin","DBTween","GameplayCameras",
			"MoviePlayer", "Http","Json", "JsonUtilities", "OnlineSubsystem", "OnlineSubsystemUtils",
			"PlayFabCommon", "PlayFabCpp" , "PlayFab"
		});

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemIOS", "IOSAdvertising" });
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.Add("AndroidPermission");
			PrivateDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
			PrivateDependencyModuleNames.Add("AndroidAdvertising");
			
			  string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Android_UPL.xml"));
		}

	}
}
