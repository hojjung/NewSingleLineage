// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/MyJrpg.h"
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
	AStructureActor* m_Foundation;
	UPROPERTY()
	TArray<AStructureActor*> m_Walls;
	UPROPERTY()
	AInteractActorBase* m_Prop;
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
	TArray<AStructureActor*> m_Walls;
};

UCLASS()
class MYJRPG_API UConstructionManager : public UObject
{
	GENERATED_BODY()

public:
	enum class EWallDir
	{
		N,E,S,W,Length
	};

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
	AStructureActor* m_PreviewActor;
	UPROPERTY()
	AGridActor* m_GridMesh;

	TArray<FBuildDataRow*> m_AryBuildDatas;

	FConEle m_Grid[FGlobalVariable::GRID_COUNT][FGlobalVariable::GRID_COUNT];

	UPROPERTY()
	TArray<FWallAry> m_WallHorizontal; //[FGlobalVariable::GRID_COUNT + 1][FGlobalVariable::GRID_COUNT + 2];
	UPROPERTY()
	TArray<FWallAry> m_WallVertical; //[FGlobalVariable::GRID_COUNT + 2][FGlobalVariable::GRID_COUNT + 1];

	EWallDir m_Dir;
private:
	FVector GetWorldPos(int x, int y);

	void GetIndex(const FVector& inloc, int& outX, int& outY);

	bool GetWallIndex(const FVector& inloc, int& outX, int& outY, bool &isHori);
	
	bool FindEmptyWallPlace(const FConEle& Ele, EWallDir& dir);

	bool GetEmptyFoundationLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot);

	FVector GetWallWorldPos(bool isHori, int wall_x, int wall_y);

	bool GetEmptyWallLoc(int x, int y, bool isHori, FVector& outEmptyLoc, FRotator& outEmptyRot);
	
	bool GetEmptyLoc(const FVector& inloc, FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t, EWallDir dir);

	void CheckBuildable();

	bool IsBuildable();

	int GetPreviewRotDir();

public:
	void Init();
	
	void SpawnPreviewActor(FVector loc, const FBuildDataRow* dataRow = nullptr);
	
	void LoadConstruction();

	FORCEINLINE const TArray<FBuildDataRow*>& GetAryBuildDatas() const
	{
		return 	m_AryBuildDatas;
	}

	void EndBuilding();
	
	void StartBuilding();
public:
	void Cancel();
	
	void ConfirmBuild();
	
	void Rotate();

	AStructureActor* GetPreview();
};

//건설 버튼을 누루면
//건설 할 수있는 모든 구조물이 보인다.
//구조물 아이콘을 터치하면 플레이어 발근처에서
//비어있는 해당 구조물 칸에 포지션 스타트
//이후 초록색 메테리얼, 지을수 없다면 빨간색 메테리얼
//초록색일때 터치하면 프리뷰 액터가 정식액터가 되어 배정되고
//프리뷰 액터는 널처리

