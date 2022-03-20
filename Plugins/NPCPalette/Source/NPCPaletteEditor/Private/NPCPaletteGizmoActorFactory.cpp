// My First Hack n Slash


#include "NPCPaletteGizmoActorFactory.h"

#include "EditorModeManager.h"
#include "NPCPaletteEditorEdMode.h"
#include "NPCPaletteEditorEdModeToolkit.h"
#include "NPCPaletteGizmoActor.h"
#include "Components/BillboardComponent.h"

AActor* UNPCPaletteGizmoActorFactory::SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform,
	const FActorSpawnParameters& InSpawnParams)
{
	GLevelEditorModeTools().ActivateMode(FNPCPaletteEdMode::EM_NPCPaletteEdModeId);

	FNPCPaletteEdMode* EdMode = (FNPCPaletteEdMode*)GLevelEditorModeTools().GetActiveMode(FNPCPaletteEdMode::EM_NPCPaletteEdModeId);

	FActorSpawnParameters SpawnInfo = InSpawnParams;

	ANPCPaletteGizmoActor* SpawnnedActor =Cast<ANPCPaletteGizmoActor>(InLevel->OwningWorld->SpawnActor(ANPCPaletteGizmoActor::StaticClass(), &InTransform, SpawnInfo));

	return SpawnnedActor;
}

bool UNPCPaletteGizmoActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	return true;
}

AActor* UNPCPaletteGizmoActorFactory::GetDefaultActor(const FAssetData& AssetData)
{
	UClass* npcClass =ANPCPaletteGizmoActor::StaticClass();
	
	return npcClass->GetDefaultObject<ANPCPaletteGizmoActor>() ;
}
