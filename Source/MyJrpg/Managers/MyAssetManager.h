// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/DataTables/UnitEntityData.h"

#include "MyAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UMyAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UMyAssetManager() {}
	
	static UMyAssetManager* Get();

protected:
	TSet<TSharedPtr<FStreamableHandle>> m_SetUnits;

	TSet<TSharedPtr<FStreamableHandle>> m_SetGather;

public:
	TSharedPtr<FStreamableHandle> LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath);

	UTexture2D* LoadTexture(TSoftObjectPtr<UTexture2D> assetSoftPath);
	
	const UUnitEntityAsset* LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> asset);

	const UGatherAsset* LoadGatherAsset(TSoftObjectPtr<UGatherAsset> asset);

	void ClearUnits();
};

