// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyJrpgEditorTarget : TargetRules
{
	public MyJrpgEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "MyJrpg" } );
		ExtraModuleNames.AddRange(new string[] {"MyJrpgEditor"});
	}
}
