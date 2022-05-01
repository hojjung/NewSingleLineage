// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCPaletteDataAsset.h"
#include "Engine/DataTable.h"
#include "MyJrpg/Actors/Field/Gather/TreeBase.h"
#include "UObject/NoExportTypes.h"
#include "GatherTable.generated.h"

/**
 * DataTable'/Game/00_DataTables/Gather/GatherTable.GatherTable'
 */
UCLASS()
class MYJRPG_API UGatherTable : public UObject
{
	GENERATED_BODY()

public:
	UGatherTable();

	static UDataTable* GetGatherTable;
};


UCLASS(Blueprintable, hidecategories = (Object, Actor, Advanced, Navigation))
class MYJRPG_API UGatherAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public://Visual
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* m_TopMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* m_BtmMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_AnimGatherMotion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* m_SoundGatherEnd;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Gather", GetFName());
	}
};

USTRUCT()
struct FGatherDataRow : public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATreeBase> m_ClassActor = ATreeBase::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UGatherAsset> m_GatherAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fDisappearDelay = 12;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector m_Offset;
};