// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Actors/Field/Build/BuildInteract/BuildInteractBase.h"
#include "UObject/NoExportTypes.h"
#include "BuildData.generated.h"

class UInteractBase;
class AStructureActor;
/**
 * 
 */
UCLASS()
class MYJRPG_API UBuildData : public UObject
{
	GENERATED_BODY()

public:
	UBuildData();
	
	static UDataTable* GetBuildTable;
};

USTRUCT()
struct FBuildDataRow : public FCraftable
{
	GENERATED_BODY()

public:
	FBuildDataRow();

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_RowID = TEXT("Same As Key");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EBuildType m_BuildType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bSupportRotate = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_bIsFocusable = true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bIsVolatile = false;//its for evet corpse, box like that
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBuildInteractBase> m_ClassInter = nullptr;
	UPROPERTY(EditAnywhere, meta=(EditCondition = "m_ClassInter != nullptr", EditConditionHides))
	TArray<FString>  m_AryInteractVariable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AStructureActor> m_ClassActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> m_Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* m_MapIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector m_Offset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "0.1"))
	float m_fIndicatorRadius = 34.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fRotYaw = 0.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_NextUpgradeActorID;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nMaxOwnedCount = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bShowInPanel = true;

	virtual UStaticMesh* GetStMesh() override;

	virtual float GetMeshScale() override;
};

USTRUCT(BlueprintType)
struct FBuildDataHandle : public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()

public:
	FBuildDataHandle()
	{
		DataTable = UBuildData::GetBuildTable;
	}
};