#pragma once

#include "Engine.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "UnrealEd.h"

class OnDataTableChanged;
//DECLARE_LOG_CATEGORY_EXTERN (FFantasySurvivalEditorModule, All, All);
class  FMyJrpgEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	TSharedPtr<OnDataTableChanged> m_OnDataTableChanged;

	void RefreshPlayerSkillData();
};


