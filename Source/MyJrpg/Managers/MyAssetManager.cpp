#include "MyAssetManager.h"

#include "MyGameInstance.h"
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

UUnitEntityAsset* UMyAssetManager::LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> assetSoftPath)
{
	if(m_MapUnits.Contains(assetSoftPath.Get()))
	{
		m_MapUnits[assetSoftPath.Get()]++;
		return assetSoftPath.Get();
	}
	LoadPrimaryAsset(assetSoftPath.Get()->GetPrimaryAssetId());
	
	m_MapUnits.Add(assetSoftPath.Get(),1);
	
	m_AryUnits.Add(assetSoftPath.Get());
	
	return assetSoftPath.Get();
}//
void UMyAssetManager::UnloadUnit(UUnitEntityAsset* asset)
{
	if(!m_MapUnits.Contains(asset))
	{
		return;
	}
	
	int& Count = m_MapUnits[asset];
	
	Count--;
	
	if (Count <= 0)
	{
		m_MapUnits.Remove(asset);
		
		m_AryUnits.Remove(asset);
	}
	
	UKismetSystemLibrary::UnloadPrimaryAsset(asset->GetPrimaryAssetId());
	
	UKismetSystemLibrary::CollectGarbage();
}

void UMyAssetManager::ClearUnits()
{
	for(UUnitEntityAsset* Unit : m_AryUnits)
	{
		UKismetSystemLibrary::UnloadPrimaryAsset(Unit->GetPrimaryAssetId());
	}
	
	m_AryUnits.Reset();

	m_MapUnits.Reset();

	UKismetSystemLibrary::CollectGarbage();
}