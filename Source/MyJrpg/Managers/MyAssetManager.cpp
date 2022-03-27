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

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	m_AryOptions.Reset(6);
	m_AryOptions.Add(TEXT("op1"));
	m_AryOptions.Add(TEXT("op2"));
	m_AryOptions.Add(TEXT("op3"));
	m_AryOptions.Add(TEXT("op4"));
	m_AryOptions.Add(TEXT("op5"));
	m_AryOptions.Add(TEXT("op6"));
}

UUnitEntityAsset* UMyAssetManager::LoadUnitAsset(UUnitEntityAsset* asset)
{
	if(m_MapUnits.Contains(asset))
	{
		m_MapUnits[asset]++;
		return asset;
	}

	LoadPrimaryAsset(asset->GetPrimaryAssetId(),m_AryOptions);

	m_MapUnits.Add(asset,1);
	
	m_AryUnits.Add(asset);
	
	return asset;
}//
TSharedPtr<FStreamableHandle> UMyAssetManager::LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath)
{
	TSharedPtr<FStreamableHandle> Handle;
		
	GetStreamableManager().LoadSynchronous<UAnimMontage>(assetSoftPath.ToSoftObjectPath(),true,&Handle);
	
	return Handle;
}

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