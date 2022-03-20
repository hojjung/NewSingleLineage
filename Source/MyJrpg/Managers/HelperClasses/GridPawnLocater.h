// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Pawns/MonsterPawn.h"


struct GridNode
{
protected:
	TSet<AMonsterPawn*> m_SetHasPawn;

	TArray<AMonsterPawn*> m_AryPawn;

public:
	bool AddPawn(AMonsterPawn* wantAdd)
	{
		if (m_SetHasPawn.Contains(wantAdd))
		{
			return false;
		}
		m_SetHasPawn.Add(wantAdd);

		m_AryPawn.Add(wantAdd);

		return true;
	}

	void RemovePawn(AMonsterPawn* wantRmv)
	{
		m_SetHasPawn.Remove(wantRmv);

		m_AryPawn.Remove(wantRmv);
	}

	TArray<AMonsterPawn*>& GetAryPawns()
	{
		return m_AryPawn;
	}

	void ClearPtr()
	{
		m_SetHasPawn.Empty();
		m_AryPawn.Empty();
	}
};


class MYJRPG_API GridPawnLocater
{
public:
	GridPawnLocater();
	~GridPawnLocater();
	//
protected:
	GridNode m_GridPawnLocate[5][5];

	TMap<AMonsterPawn*, GridNode*> m_MapMobIndex;

	FVector m_NavBoundExtent;

	float m_NodeX;

	float m_NodeY;

protected:
	GridNode& GetGridNode(const AMonsterPawn* want);

public:
	void Init(const TArray<AMonsterPawn*>& aryMobs);

	void AddPawn(AMonsterPawn* wantAdd);

	const TArray<AMonsterPawn*>& GetNodeMobs(const AMonsterPawn* wantAdd);
};