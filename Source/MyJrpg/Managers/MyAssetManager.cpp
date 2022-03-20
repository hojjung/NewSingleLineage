#include "MyAssetManager.h"
#include "Engine/Engine.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/PortraitData.h"

UMyAssetManager* UMyAssetManager::Get()
{
	UMyAssetManager* This = Cast<UMyAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return This;
	}
	else
	{
		PRINTF("ERROR:AssetManagerSetNeed");
		return nullptr; // never calls this
	}
}

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	m_UnitMap.Reset();
	m_ItemMap.Reset();
}

UTexture2D* UMyAssetManager::LoadIconAsset(TSoftObjectPtr<UTexture2D> assetSoftPath)
{
	FSoftObjectPath SoftPath = assetSoftPath.ToSoftObjectPath();

	TSharedPtr<FStreamableHandle> Handle;
		
	return GetStreamableManager().LoadSynchronous<UTexture2D>(assetSoftPath.ToSoftObjectPath(),true,&Handle);
}

UUnitEntityAsset* UMyAssetManager::LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> assetSoftPath)
{
	FSoftObjectPath SoftPath = assetSoftPath.ToSoftObjectPath();

	UUnitEntityAsset* EntityAssetWantUse = nullptr;
	
	if(m_UnitMap.Find(SoftPath))
	{
		EntityAssetWantUse = Cast<UUnitEntityAsset>( m_UnitMap[SoftPath].Get()->GetLoadedAsset());
	}
	else
	{
		TSharedPtr<FStreamableHandle> Handle;
		
		EntityAssetWantUse = GetStreamableManager().LoadSynchronous<UUnitEntityAsset>(assetSoftPath.ToSoftObjectPath(),true,&Handle);
		
		m_UnitMap.Add(SoftPath,Handle);
	}

	return EntityAssetWantUse;
}//

UUnitEntityAsset* UMyAssetManager::LoadUnitAssetWithID(FName unitID)
{
	FUnitEntityRow* FoundRow = nullptr;

	FoundRow = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(unitID, "");
	
	if(!FoundRow)
	{
		return nullptr;
	}
	
	return LoadUnitAsset(FoundRow->m_UnitDataAsset);
}

TSharedPtr<FStreamableHandle> UMyAssetManager::LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath)
{
	TSharedPtr<FStreamableHandle> Handle;
		
	GetStreamableManager().LoadSynchronous<UAnimMontage>(assetSoftPath.ToSoftObjectPath(),true,&Handle);

	return Handle;
}

UParticleSystem* UMyAssetManager::LoadParticleEffect(TSoftObjectPtr<UParticleSystem> assetSoftPath)
{
	FSoftObjectPath SoftPath = assetSoftPath.ToSoftObjectPath();

	UParticleSystem* EntityAssetWantUse = nullptr;
	
	if(m_ParticleEffectMap.Find(SoftPath))
	{
		EntityAssetWantUse = Cast<UParticleSystem>( m_ParticleEffectMap[SoftPath].Get()->GetLoadedAsset());
	}
	else
	{
		TSharedPtr<FStreamableHandle> Handle;
		
		EntityAssetWantUse = GetStreamableManager().LoadSynchronous<UParticleSystem>(assetSoftPath.ToSoftObjectPath(),true,&Handle);
		
		m_ParticleEffectMap.Add(SoftPath,Handle);
	}

	return EntityAssetWantUse;
}

void UMyAssetManager::ClearUnitAsset()
{
	for(auto& Iter : m_UnitMap)
	{
		Iter.Value.Get()->ReleaseHandle();
	}

	m_UnitMap.Reset();

	CollectGarbage(EObjectFlags::RF_Public);
}

void UMyAssetManager::ClearItemAsset()
{
	for(auto& Iter : m_ItemMap)
	{
		Iter.Value.Get()->ReleaseHandle();
	}

	m_ItemMap.Reset();

	CollectGarbage(EObjectFlags::RF_Public);
}

void UMyAssetManager::ClearEffectAsset()
{
	for(auto& Iter : m_ParticleEffectMap)
	{
		Iter.Value.Get()->ReleaseHandle();
	}

	m_ParticleEffectMap.Reset();

	CollectGarbage(EObjectFlags::RF_Public);
}
