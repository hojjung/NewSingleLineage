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
	TMap<FSoftObjectPath,TSharedPtr<FStreamableHandle>> m_UnitMap;

	TMap<FSoftObjectPath,TSharedPtr<FStreamableHandle>> m_ItemMap;

	TMap<FSoftObjectPath,TSharedPtr<FStreamableHandle>> m_ParticleEffectMap;

public:
	virtual void StartInitialLoading() override;

public:
	UTexture2D* LoadIconAsset(TSoftObjectPtr<UTexture2D> assetSoftPath);

	UUnitEntityAsset* LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> assetSoftPath);

	UUnitEntityAsset* LoadUnitAssetWithID(FName unitID);

	TSharedPtr<FStreamableHandle> LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath);

	UParticleSystem* LoadParticleEffect(TSoftObjectPtr<UParticleSystem> assetSoftPath);
	
public:
	void ClearUnitAsset();

	void ClearItemAsset();

	void ClearEffectAsset();
};

