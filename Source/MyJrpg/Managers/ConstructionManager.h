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

UCLASS()
class MYJRPG_API UConstructionManager : public UObject
{
	GENERATED_BODY()

public:
	enum class EWallDir
	{
		N,S,W,E,Length
	};

	UConstructionManager();

protected:
	UPROPERTY()
	UMaterialInterface* m_MatGreen;
	UPROPERTY()
	UMaterialInterface* m_MatRed;
	UPROPERTY()
	UMaterialInterface* m_MatGrid;
	UPROPERTY()
	AStructureActor* m_PreviewActor;
	UPROPERTY()
	AStaticMeshActor* m_GridMesh;

	TArray<FBuildDataRow*> m_AryBuildDatas;

	FConEle m_Grid[FGlobalVariable::GRID_COUNT][FGlobalVariable::GRID_COUNT];

protected:
	bool FindEmptyWallPlace(const FConEle& Ele, EWallDir& dir);

	bool GetEmptyFoundationLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot);

	bool GetEmptyWallLoc(int x, int y, FVector& outEmptyLoc, FRotator& outEmptyRot);
	
	bool GetEmptyLoc(FVector& outEmptyLoc, FRotator& outEmptyRot, EBuildType t);

	void CheckBuildable();

	bool IsBuildable();

public:
	void Init();
	
	void SpawnPreviewActor(const FBuildDataRow& dataRow);

	void ConfirmBuild();
	
	void LoadConstruction();

	void RemoveConstruction();

	FORCEINLINE const TArray<FBuildDataRow*>& GetAryBuildDatas() const
	{
		return 	m_AryBuildDatas;
	}
};

//건설 버튼을 누루면
//건설 할 수있는 모든 구조물이 보인다.
//구조물 아이콘을 터치하면 플레이어 발근처에서
//비어있는 해당 구조물 칸에 포지션 스타트
//이후 초록색 메테리얼, 지을수 없다면 빨간색 메테리얼
//초록색일때 터치하면 프리뷰 액터가 정식액터가 되어 배정되고
//프리뷰 액터는 널처리