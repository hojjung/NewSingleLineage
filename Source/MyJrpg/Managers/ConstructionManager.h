// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Interfaces/Buildable.h"
#include "UObject/NoExportTypes.h"
#include "ConstructionManager.generated.h"

/**
 * 격자 만들기
 * 프리뷰 액터 격자에 스냅 생성시켜주기
 * 
 */

class AGridActor;
class AStaticMeshActor;
class AInteractActorBase;
class AStructureActor;
struct FBuildDataRow;

USTRUCT()
struct FConEle
{
	GENERATED_BODY()

public:
	FConEle();

public:
	UPROPERTY()
	TScriptInterface<IBuildable> m_Foundation;
	UPROPERTY()
	TScriptInterface<IBuildable> m_Prop;
};

USTRUCT()
struct FWallAry
{
	GENERATED_BODY()

public:
	FWallAry();

	FWallAry(int count);

public:
	UPROPERTY()
	TArray<TScriptInterface<IBuildable>> m_Walls;
};

UCLASS()
class MYJRPG_API UConstructionManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnBuildDone);

	FOnBuildDone m_OnConfirm;

	FOnBuildDone m_OnCancel;

	UConstructionManager();

private:
	UPROPERTY()
	UMaterialInterface* m_MatGreen;
	UPROPERTY()
	UMaterialInterface* m_MatRed;
	UPROPERTY()
	AGridActor* m_GridMesh;
	UPROPERTY()
	TScriptInterface<IBuildable> m_PreviewActor;
	UPROPERTY()
	TScriptInterface<IBuildable>  m_FocusActor;

	TArray<FBuildDataRow*> m_AryBuildDatas;

	FConEle m_Grid[FGlobalVariable::GRID_COUNT][FGlobalVariable::GRID_COUNT];

	UPROPERTY()
	TArray<FWallAry> m_WallHorizontal; //[FGlobalVariable::GRID_COUNT + 1][FGlobalVariable::GRID_COUNT + 2];
	UPROPERTY()
	TArray<FWallAry> m_WallVertical; //[FGlobalVariable::GRID_COUNT + 2][FGlobalVariable::GRID_COUNT + 1];

private:
	FVector GetWorldPos(int x, int y);

	void GetIndex(const FVector& inloc, int& outX, int& outY);

	bool GetWallIndex(const FVector& inloc, int& outX, int& outY, bool &isHori);

	int GetWallMaxIndex(bool ishori, bool isXaxis);
	
	bool GetEmptyFoundationLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot);

	FVector GetWallWorldPos(bool isHori, int wall_x, int wall_y);

	bool GetEmptyWallLoc(int x, int y, bool isHori, FVector& outEmptyLoc, FRotator& outEmptyRot);
	
	bool GetEmptyLoc(const FVector& inloc, FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t);

	void CheckBuildable();

	bool IsBuildable();

	IBuildable* SpawnStructure(const FBuildDataRow& data);

public:
	void Init();
	
	void SpawnPreviewActor(FVector loc, const FBuildDataRow* dataRow = nullptr);
	
	void LoadConstruction();

	FORCEINLINE const TArray<FBuildDataRow*>& GetAryBuildDatas() const
	{
		return 	m_AryBuildDatas;
	}

	void StartBuilding();
	
	void EndBuilding();
	
public:
	void Cancel();
	
	void ConfirmBuild();
	
	void Rotate();

	IBuildable* GetPreview();
	
	void SelectStruct(IBuildable* sActor);

	void CancelSelect();
	
	void Erase(IBuildable* buildActor);
	
	void Upgrade(IBuildable* buildActor);

	void GetStructureHolder(IBuildable* want, TScriptInterface<IBuildable> *& holder, bool &isHori);
};

//건설 버튼을 누루면
//건설 할 수있는 모든 구조물이 보인다.
//구조물 아이콘을 터치하면 플레이어 발근처에서
//비어있는 해당 구조물 칸에 포지션 스타트
//이후 초록색 메테리얼, 지을수 없다면 빨간색 메테리얼
//초록색일때 터치하면 프리뷰 액터가 정식액터가 되어 배정되고
//프리뷰 액터는 널처리

