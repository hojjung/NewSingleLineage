// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawnLocater.h"

GridPawnLocater::GridPawnLocater(): m_NodeX(0), m_NodeY(0)
{
	m_MapMobIndex.Reserve(40);
}

GridPawnLocater::~GridPawnLocater()
{
	int IterX = 0;
	int IterY = 0;

	while (IterX < 5)
	{
		while (IterY < 5)
		{
			m_GridPawnLocate[IterX][IterY].ClearPtr();
			IterY++;
		}
		IterY = 0;
		IterX++;
	}

	m_MapMobIndex.Empty();
}

GridNode& GridPawnLocater::GetGridNode(const AMonsterPawn* want)
{
	FVector CrntLoc = want->GetActorLocation();

	int Column = CrntLoc.X;
	int Row = CrntLoc.Y;

	Column += m_NavBoundExtent.X;
	Row += m_NavBoundExtent.Y;

	Column /= m_NodeX;
	Row /= m_NodeY;

	return m_GridPawnLocate[Column][Row];
}

void GridPawnLocater::Init(const TArray<AMonsterPawn*>& aryMobs)
{
	for (AMonsterPawn* Mob : aryMobs)
	{
		m_MapMobIndex.Add(Mob, &m_GridPawnLocate[0][0]);
	}

	TArray<FBox> Bound = UMyLib::GetNavSys()->MainNavData->GetNavigableBounds();

	m_NavBoundExtent = Bound[0].GetExtent();

	m_NodeX = (m_NavBoundExtent.X * 2.f) / 5;

	m_NodeY = (m_NavBoundExtent.Y * 2.f) / 5;
}

void GridPawnLocater::AddPawn(AMonsterPawn* wantAdd)
{
	GridNode& CrntNode = GetGridNode(wantAdd);

	if (CrntNode.AddPawn(wantAdd))
	{
		m_MapMobIndex[wantAdd]->RemovePawn(wantAdd);

		m_MapMobIndex[wantAdd] = &CrntNode;
	}
}

const TArray<AMonsterPawn*>& GridPawnLocater::GetNodeMobs(const AMonsterPawn* wantAdd)
{
	return GetGridNode(wantAdd).GetAryPawns();
}

