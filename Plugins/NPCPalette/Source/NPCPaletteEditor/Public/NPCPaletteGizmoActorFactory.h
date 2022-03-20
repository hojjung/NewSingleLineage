// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "ActorFactories/ActorFactory.h"
#include "NPCPaletteGizmoActorFactory.generated.h"

/**
 * 
 */
UCLASS()
class NPCPALETTEEDITOR_API UNPCPaletteGizmoActorFactory : public UActorFactory
{
	GENERATED_BODY()

public:
	virtual AActor* SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform, const FActorSpawnParameters& InSpawnParams) override;

	virtual bool CanCreateActorFrom( const FAssetData& AssetData, FText& OutErrorMsg ) override;

	virtual AActor* GetDefaultActor( const FAssetData& AssetData ) override;
};
