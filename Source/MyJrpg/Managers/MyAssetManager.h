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
	UPROPERTY()
	TArray<UUnitEntityAsset*> m_AryUnits;
	
	TMap<UUnitEntityAsset*,int> m_MapUnits;

	TMap<FSoftObjectPath,TSharedPtr<FStreamableHandle>> m_ParticleEffectMap;

	TArray<FName> m_AryOptions;

protected:
	virtual void StartInitialLoading() override;

public:
	UUnitEntityAsset* LoadUnitAsset(UUnitEntityAsset* asset);

	TSharedPtr<FStreamableHandle> LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath);

	void UnloadUnit(UUnitEntityAsset* asset);

	void ClearUnits();
};

