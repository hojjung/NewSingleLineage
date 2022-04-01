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
	TArray<FName> m_AryOptions;

protected:
	virtual void StartInitialLoading() override;

public:
	TSharedPtr<FStreamableHandle> LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath);
	
	void LoadUnitAsset(const UUnitEntityAsset* asset, FStreamableDelegate dele);

	void UnloadUnit(const UUnitEntityAsset* asset);

	void ClearUnits();
};

