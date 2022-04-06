// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
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

public:
	TSharedPtr<FStreamableHandle> LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath);
	
	const UUnitEntityAsset* LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> asset);

	void ClearUnits();
};

