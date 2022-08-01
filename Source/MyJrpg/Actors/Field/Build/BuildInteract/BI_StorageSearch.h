// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BI_Storage.h"
#include "BI_StorageSearch.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UBI_StorageSearch : public UBI_Storage
{
	GENERATED_BODY()

protected:
	bool m_bIsSearched;

protected:
	void OnSearchEnd();
	
	virtual void Init(const TArray<FString>& variable, UInventory* inven) override;
	
	virtual bool IsEraseable() override;

	virtual void OnInteract() override;

public:
	bool IsSearched();
	
	void SetSearched(bool is_searched);
};
