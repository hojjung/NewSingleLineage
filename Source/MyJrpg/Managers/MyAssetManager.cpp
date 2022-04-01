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

void UMyAssetManager::LoadUnitAsset(const UUnitEntityAsset* asset, FStreamableDelegate dele)
{
	LoadPrimaryAsset(asset->GetPrimaryAssetId(),m_AryOptions,dele);
}//
TSharedPtr<FStreamableHandle> UMyAssetManager::LoadAnimMontage(TSoftObjectPtr<UAnimMontage> assetSoftPath)
{
	TSharedPtr<FStreamableHandle> Handle;
		
	GetStreamableManager().LoadSynchronous<UAnimMontage>(assetSoftPath.ToSoftObjectPath(),true,&Handle);
	
	return Handle;
}

void UMyAssetManager::UnloadUnit(const UUnitEntityAsset* asset)
{
	UKismetSystemLibrary::UnloadPrimaryAsset(asset->GetPrimaryAssetId());
}

void UMyAssetManager::ClearUnits()
{
	UKismetSystemLibrary::CollectGarbage();
}
