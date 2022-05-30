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

public:
	UBI_StorageSearch();

protected:
	UPROPERTY(VisibleAnywhere)
   	UAnimMontage* m_Anim;
	
	bool m_bIsSearched;

protected:
	void OnSearchEnd();
	
	virtual void Init() override;
	
	virtual bool IsEraseable() override;

	virtual void OnInteract() override;
};
