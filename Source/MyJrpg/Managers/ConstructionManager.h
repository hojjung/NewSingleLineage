// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraftManager.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Widgets/World/Build/WidgetBuildElement.h"
#include "UObject/NoExportTypes.h"
#include "ConstructionManager.generated.h"

/**
 * 격자 만들기
 * 프리뷰 액터 격자에 스냅 생성시켜주기
 * 
 */

class AGridActor;
class AStaticMeshActor;
class AStructureActor;
struct FBuildDataRow;

USTRUCT()
struct FConEle
{
	GENERATED_BODY()

public:
	FConEle();

public:
	
	TWeakObjectPtr<AStructureActor> m_Foundation;

	
	TWeakObjectPtr<AStructureActor> m_Furniture;
};

USTRUCT()
struct FWallAry
{
	GENERATED_BODY()

public:
	FWallAry();

	FWallAry(int count);

public:
	
	TArray<TWeakObjectPtr<AStructureActor>> m_Walls;
};

UCLASS()
class MYJRPG_API UConstructionManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnBuildDone);

	FOnBuildDone m_OnConfirm;

	FOnBuildDone m_OnCancel;

	FOnBuildDone m_OnChanged;

	UConstructionManager();

private:
	UPROPERTY()
	UMaterialInterface* m_MatGreen;
	UPROPERTY()
	UMaterialInterface* m_MatRed;
	UPROPERTY()
	UMaterialInterface* m_MatCyan;
	UPROPERTY()
	AGridActor* m_GridMesh;
	
	TWeakObjectPtr<AStructureActor> m_PreviewActor;
	
	TWeakObjectPtr<AStructureActor>  m_FocusActor;

	TArray<FBuildDataRow*> m_AryBuildDatas;

	FConEle m_Grid[FGlobalVariable::GRID_COUNT][FGlobalVariable::GRID_COUNT];

	UPROPERTY()
	TArray<FWallAry> m_WallHorizontal; //[FGlobalVariable::GRID_COUNT + 1][FGlobalVariable::GRID_COUNT + 2];
	UPROPERTY()
	TArray<FWallAry> m_WallVertical; //[FGlobalVariable::GRID_COUNT + 2][FGlobalVariable::GRID_COUNT + 1];

	TMap<FName,int> m_InvenFurniture;

private:
	FVector GetWorldPos(int x, int y);

	void GetIndex(const FVector& inloc, int& outX, int& outY);

	bool GetWallIndex(const FVector& inloc, int& outX, int& outY, bool &isHori);

	int GetWallMaxIndex(bool ishori, bool isXaxis);
	
	bool GetEmptyFoundationLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot);

	bool GetEmptyFurnitureLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot);

	FVector GetWallWorldPos(bool isHori, int wall_x, int wall_y);

	bool GetEmptyWallLoc(int x, int y, bool isHori, FVector& outEmptyLoc, FRotator& outEmptyRot);
	
	bool GetEmptyLoc(const FVector& inloc, FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t);

	void CheckBuildable();

	bool IsBuildable();

	AStructureActor* SpawnStructure(const FBuildDataRow& data);

	bool IsEraseable();

	void OnErase(const FVector& Loc);

	void TryEraseActor(TWeakObjectPtr<AStructureActor>& holder);

	bool TraceBuildable(const FVector& Loc, const FVector&& extent, const FRotator& rot, float height);

	void SetFurnitureWallShow();

public:
	void Init();
	
	void SetFurnitureHide();
	
	void SetWallStructureHide();
	
	void SpawnPreviewActor(FVector loc, const FBuildDataRow* dataRow = nullptr);
	
	void LoadConstruction();

	FORCEINLINE const TArray<FBuildDataRow*>& GetAryBuildDatas() const
	{
		return 	m_AryBuildDatas;
	}

	void StartBuilding();
	
	void EndBuilding();
	
	void Cancel(); 
	
	void ConfirmBuild();
	
	void Rotate();

	AStructureActor* GetPreview();
	
	void SelectStruct(AStructureActor* sActor);

	void CancelSelect();
	
	void Erase(AStructureActor* buildActor);
	
	void Upgrade(AStructureActor* buildActor);

	void GetStructureHolder(AStructureActor* want, TWeakObjectPtr<AStructureActor> *& holder, bool &isHori);
	
	void AddFurniture(const FName& id);

	const TMap<FName,int>& GetInvenFurniture() const;
};


