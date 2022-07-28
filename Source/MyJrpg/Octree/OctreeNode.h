// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyJrpg/Interfaces/Focusable.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

/**
 * 
 */
class MYJRPG_API OctreeNode : public TSharedFromThis<OctreeNode>
{
public:
	bool m_bIsRange;
	float m_fMiniSize = 20;
	
	int32 m_nMaxCount = 4;
	int32 m_nDepth;
	
	FVector m_Center;
	FVector m_Extend;

	TSharedPtr<OctreeNode> m_Root;
	TArray<AActor*> m_AryActors;
	TArray<TSharedPtr<OctreeNode>> m_AryChildren;
	
public:
	OctreeNode(FVector _center, FVector _extend, int32 _depth, TSharedPtr<OctreeNode> _root = nullptr)
		: m_Center(_center), m_Extend(_extend), m_nDepth(_depth)
	{
		m_Root = _root;
		m_bIsRange = false;
	}

	~OctreeNode()
	{
		m_Root = nullptr;
		m_AryActors.Empty();
		m_AryChildren.Empty();
	}

	bool IsNotUsed()
	{
		return true;
	}

	bool InterSection(FVector _OCenter, float _radian)
	{
		FVector v = _OCenter - m_Center;
		float x = UKismetMathLibrary::Min(v.X, m_Extend.X);
		x = UKismetMathLibrary::Max(x, -m_Extend.X);

		float y = UKismetMathLibrary::Min(v.Y, m_Extend.Y);
		y = UKismetMathLibrary::Max(y, -m_Extend.Y);

		return (x - v.X) * (x - v.X) + (y - v.Y) * (y - v.Y) <= _radian * _radian;
	}

	bool InterSection(FVector _point)
	{
		return (_point.X >= m_Center.X - m_Extend.X &&
			_point.X <= m_Center.X + m_Extend.X &&
			_point.Y >= m_Center.Y - m_Extend.Y &&
			_point.Y <= m_Center.Y + m_Extend.Y
		);
	}
	
	int SelectBestChild(FVector _point)
	{
		return (_point.X <= m_Center.X ? 0 : 1) + (_point.Y >= m_Center.Y ? 0 : 2);
	}

	void split()
	{
		float quarter = m_Extend.X / 2.0f;
		m_Root = m_Root.IsValid() ? m_Root : this->AsShared();
		m_AryChildren.Init(nullptr, 4);
		m_AryChildren[0] = MakeShareable(new OctreeNode(m_Center + FVector(-quarter, quarter, m_Center.Z), m_Extend / 2,m_nDepth + 1, m_Root));
		m_AryChildren[1] = MakeShareable(new OctreeNode(m_Center + FVector(quarter, quarter, m_Center.Z), m_Extend / 2,m_nDepth + 1, m_Root));
		m_AryChildren[2] = MakeShareable(new OctreeNode(m_Center + FVector(-quarter, -quarter, m_Center.Z), m_Extend / 2,m_nDepth + 1, m_Root));
		m_AryChildren[3] = MakeShareable(new OctreeNode(m_Center + FVector(quarter, -quarter, m_Center.Z), m_Extend / 2,m_nDepth + 1, m_Root));
	}

	void InsertObject(AActor* obj)
	{
		if (obj == nullptr || !InterSection(obj->GetActorLocation()))
			return;

		int32 childIndex = 0;
		
		if (m_AryChildren.Num() == 0)
		{
			if (m_AryActors.Num() <= m_nMaxCount || m_Extend.X <= m_fMiniSize)
			{
				m_AryActors.Add(obj);
				return;
			}
			split();

			for (int32 i = m_AryActors.Num() - 1; i >= 0; i--)
			{
				childIndex = SelectBestChild(m_AryActors[i]->GetActorLocation());
				m_AryChildren[childIndex]->InsertObject(m_AryActors[i]);
				m_AryActors.Swap(i, m_AryActors.Num() - 1);
				m_AryActors.Pop();
			}
		}
		childIndex = SelectBestChild(obj->GetActorLocation());
		m_AryChildren[childIndex]->InsertObject(obj);
	}

	bool canMerge()
	{
		int TotalObjCount = m_AryActors.Num();
		if (m_AryChildren.Num() > 0)
		{
			for (auto& child : m_AryChildren)
			{
				if (child->m_AryChildren.Num() > 0)
				{
					return false;
				}
				TotalObjCount += child->m_AryActors.Num();
			}
		}
		return TotalObjCount <= m_nMaxCount;
	}

	void Merge()
	{
		for (auto& child : m_AryChildren)
		{
			m_AryActors.Append(child->m_AryActors);
		}
		m_AryChildren.Empty();
	}

	bool RmoveObject(AActor* obj)
	{
		bool bRemove = false;
		for (int32 i = 0; i < m_AryActors.Num(); i++)
		{
			if (m_AryActors[i] == obj)
			{
				m_AryActors.RemoveSwap(obj);
				bRemove = true;
				break;
			}
		}

		if (!bRemove && m_AryChildren.Num() > 0)
		{
			int32 childIndex = SelectBestChild(obj->GetActorLocation());
			m_AryChildren[childIndex]->RmoveObject(obj);
			bRemove = true;
		}

		if (bRemove && m_AryChildren.Num() > 0 && canMerge())
		{
			Merge();
		}
		return bRemove;
	}

	void DrawBound(UObject* worldContext, float time = 0.02f, float thickness = 2.0f)
	{
		TArray<FLinearColor> colors = {
			FLinearColor(0.5, 0, 0, 1), FLinearColor(0.5, 0, 0.5, 1), FLinearColor(1, 0.5, 0, 1),
			FLinearColor(1, 0, 0, 1)
		};
		FLinearColor drawColor = m_bIsRange ? FLinearColor::Green : colors[UKismetMathLibrary::Clamp(m_nDepth, 0, 3)];
		FVector drawCenter = m_Center; // +(bInRange ? FVector(0, 0, 8) : FVector(0, 0, 5));
		UKismetSystemLibrary::DrawDebugBox(worldContext, drawCenter, m_Extend, drawColor, FRotator::ZeroRotator, time,
		                                   thickness + m_nDepth * 0.2);
	}

	template <class T>
	void TraceObjectInRange(const AActor* traceActor, float _radian, TArray<T*>& aryOut)
	{
		float RadSqr = _radian * _radian;

		FVector _OCenter = traceActor->GetActorLocation();
		m_bIsRange = false;
		if (InterSection(_OCenter, _radian))
		{
			m_bIsRange = true;

			for (int32 i = m_AryActors.Num() - 1; i >= 0; i--)
			{
				for (AActor* obj : m_AryActors)
				{
					AMyPlayerPawn* Pl = Cast<AMyPlayerPawn>(obj);

					if(Pl)
					{
						continue;
					}
					
					T* CastedObj = Cast<T>(obj);

					if (!CastedObj || obj == traceActor)
					{
						continue;
					}
					bool bCanActive = FVector::DistSquared2D(_OCenter, obj->GetActorLocation()) <= RadSqr;

					if (bCanActive)
					{
						aryOut.Add(CastedObj);
					}
				}
			}
			for (auto& child : m_AryChildren)
			{
				child->TraceObjectInRange(traceActor, _radian, aryOut);
			}
		}
		else
		{
			TraceObjectOutRange<T>(aryOut);
		}
	}

	template <class T>
	void TraceObjectOutRange(TArray<T*>& aryOut)
	{
		m_bIsRange = false;
		for (int32 i = m_AryActors.Num() - 1; i >= 0; i--)
		{
			T* CastedObj = Cast<T>(m_AryActors[i]);

			if (!CastedObj)
			{
				continue;
			}
			aryOut.Add(CastedObj);
		}
		for (auto& node : m_AryChildren)
		{
			if (node.IsValid())
			{
				node->TraceObjectOutRange(aryOut);
			}
		}
	}

	void UpdateState()
	{
		for (int32 i = m_AryActors.Num() - 1; i >= 0; i--)
		{
			if (!InterSection(m_AryActors[i]->GetActorLocation()))
			{
				AActor* obj = m_AryActors[i];
				RmoveObject(obj);
				m_Root->InsertObject(obj);
			}
		}
		if (m_AryChildren.Num() > 0)
		{
			if (canMerge())
			{
				Merge();
			}
			for (auto& child : m_AryChildren)
			{
				child->UpdateState();
			}
		}
	}

	IFocusable* GetNearTarget(AActor* self, const FVector& loc, float range, float myTargetingRange,bool isManualMode, const UClass* ignoreClass = nullptr)
	{
		OctreeNode* Start = this;
		
		float MaxRange = MAX_flt;

		if(range > 0 && isManualMode)
		{
			MaxRange = range; 
		}

		AActor* Target = nullptr;

		if (InterSection(loc, MaxRange))
		{
			for(AActor* InnerActor : Start->m_AryActors)
			{
				if((InnerActor->IsPendingKill()) || (self == InnerActor) || (InnerActor->GetClass() == ignoreClass))
				{
					continue;
				}
				IFocusable* Focus = Cast<IFocusable>(InnerActor);

				if(Focus && !Focus->IsInteractable())
				{
					continue;
				}
				if(!isManualMode)
				{
					ACombatUnitPawn* Pawn = Cast<ACombatUnitPawn>(InnerActor);

					if(Pawn && (!Pawn->IsAlive() || !UMyGameInstance::Get->m_TeamKarma->IsFoe(Pawn)))
					{
						continue;	
					}
				}
				
				float NavLen = 0.f;

				ENavigationQueryResult::Type ResultT = UMyLib::GetNavSys()->GetPathLength(self, loc, InnerActor->GetActorLocation(), NavLen);
				
				if(ResultT != ENavigationQueryResult::Success)
				{
					NavLen = MAX_flt;
				}

				if(!Cast<ACombatUnitPawn>(InnerActor) || !isManualMode)
				{
					NavLen += myTargetingRange;//사거리안에 다른 몬스터 있을때 타겟팅이 유닛 우선순위로 가게해줌
				}
				
				if(NavLen > MaxRange)
				{
					continue;
				}
				
				MaxRange = NavLen;

				Target = InnerActor;
			}

			for (auto& child : m_AryChildren)
			{
				IFocusable* ChildInner = child->GetNearTarget(self, loc, range, myTargetingRange,isManualMode, ignoreClass);

				if(!ChildInner)
				{
					continue;
				}

				AActor* ChildInnerActor = Cast<AActor>(ChildInner);

				float NavLen = 0.f;
				
				UMyLib::GetNavSys()->GetPathLength(self, loc, ChildInnerActor->GetActorLocation(), NavLen);
				
				if(NavLen > MaxRange)
				{
					continue;
				}
				MaxRange = NavLen;

				Target = ChildInnerActor;
			}
		}

		IFocusable* Focus = Cast<IFocusable>(Target);
		
		return Focus; 
	}
};
