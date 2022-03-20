// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "SNPCPaletteModeTool.h"
#include "Toolkits/BaseToolkit.h"

class FNPCPaletteEdModeToolkit : public FModeToolkit
{
public:

	FNPCPaletteEdModeToolkit()
	{
		SAssignNew( PlacementModeTools, SNPCPaletteModeTool );
	}

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override { return FName("PlacementMode"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("BuilderModeToolkit", "DisplayName", "Builder"); }
	virtual class FEdMode* GetEditorMode() const override { return GLevelEditorModeTools().GetActiveMode( FNPCPaletteEdMode::EM_NPCPaletteEdModeId ); }
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return PlacementModeTools; }

private:

	TSharedPtr<SNPCPaletteModeTool> PlacementModeTools;
};
