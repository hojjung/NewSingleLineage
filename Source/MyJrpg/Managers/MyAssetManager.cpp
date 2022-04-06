#include "MyAssetManager.h"
#include "MyGameInstance.h"
#include "Engine/Engine.h"
#include "MyJrpg/MyJrpg.h"

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

const UUnitEntityAsset* UMyAssetManager::LoadUnitAsset(TSoftObjectPtr<UUnitEntityAsset> asset)
{
	TSharedPtr<FStreamableHandle> Handle;
	
	UUnitEntityAsset* LoadedAsset = GetStreamableManager().LoadSynchronous<UUnitEntityAsset>(asset.ToSoftObjectPath(),true, &Handle);
	
	m_SetUnits.Add(Handle);

	return LoadedAsset; 
}//
TSharedPtr<FStreamableHandle> UMyAssetManager::LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath)
{
	TSharedPtr<FStreamableHandle> Handle;
		
	GetStreamableManager().LoadSynchronous<UAnimMontage>(assetSoftPath.ToSoftObjectPath(),true,&Handle);
	
	return Handle;
}

void UMyAssetManager::ClearUnits()
{
	for(auto Handle : m_SetUnits)
	{
		Handle->ReleaseHandle();
	}

	m_SetUnits.Reset();
	
	UKismetSystemLibrary::CollectGarbage();
}
