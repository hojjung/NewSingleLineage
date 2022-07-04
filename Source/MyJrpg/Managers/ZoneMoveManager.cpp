#include "ZoneMoveManager.h"

void UZoneMoveManager::Init()
{
	m_nZoneStamina  = 100;
	
	m_CurrentID = TEXT("PlayerHome");
}

void UZoneMoveManager::ClearWidgetMap()
{
	m_MapZoneBtns.Reset();
}

void UZoneMoveManager::AddMapBtn(FName zoneID, UWidgetMapBtn* mapBtn)
{
	m_MapZoneBtns.Emplace(zoneID, TWeakObjectPtr<UWidgetMapBtn>(mapBtn));
}

void UZoneMoveManager::Tick(float deltaTime)
{
	if(m_DestZoneID.IsNone())
	{
		return;
	}
	m_fRemainDuration += deltaTime;

	if(m_fRemainDuration >= m_fMaxDuration)
	{
		m_CurrentID = m_DestZoneID;
		
		m_DestZoneID = NAME_None;
		
		m_OnMoveEnd.Broadcast();

		return;
	}

	m_OnMoveTick.Broadcast();
}

bool UZoneMoveManager::TryPurchaseStamina(int want)
{
	if(m_nZoneStamina < want)
	{
		return false;
	}
	
	m_nZoneStamina -= want;

	return true;
}

void UZoneMoveManager::StartMove(bool isRunning, float timeUse, FName destZoneID)
{
	m_bIsRunning = isRunning;
	
	m_fMaxDuration = timeUse;

	m_DestZoneID = destZoneID;
	
	m_fRemainDuration = 0;
	
	m_fMaxDistance = GetDist(m_DestZoneID);

	m_OnMoveStart.Broadcast(m_DestZoneID, m_fMaxDistance);
}

float UZoneMoveManager::GetMovePercent()
{
	return m_fRemainDuration / m_fMaxDuration;
}

float UZoneMoveManager::GetDist(FName dst)
{
	FVector2D SrcPos = m_MapZoneBtns[m_CurrentID]->GetPos();
	
	FVector2D DstPos = m_MapZoneBtns[dst]->GetPos();
	
	float Dist = FVector2D::Distance(DstPos, SrcPos);

	return Dist;
}

bool UZoneMoveManager::IsZoneAlreadyIn(FName dst)
{
	return m_CurrentID == dst;
}

FVector2D UZoneMoveManager::GetBarPos(const FName& dst)
{
	FVector2D SrcPos = m_MapZoneBtns[m_CurrentID]->GetPos();
	
	FVector2D DstPos = m_MapZoneBtns[dst]->GetPos();
	
	FVector2D DirPos = (DstPos - SrcPos) / 2.f; 
		
	return SrcPos + DirPos;
}

FVector2D UZoneMoveManager::GetPlayerIconPos()
{
	FVector2D SrcPos = m_MapZoneBtns[m_CurrentID]->GetPos();
	
	FVector2D DstPos = m_MapZoneBtns[m_DestZoneID]->GetPos();

	float Percent = GetMovePercent();
	
	FVector2D DirPos = (DstPos - SrcPos) * Percent; 
		
	return SrcPos + DirPos;
}

float UZoneMoveManager::GetEulerAngle(const FName& dst)
{
	FVector2D SrcPos = m_MapZoneBtns[m_CurrentID]->GetPos();
	
	FVector2D DstPos = m_MapZoneBtns[dst]->GetPos();
	
	float Ang1 = FMath::Atan2(SrcPos.X, SrcPos.Y);
	
	float Ang2 = FMath::Atan2(DstPos.X, DstPos.Y);
	
	float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);

	Ang += 90.f;
	//if(Ang > 180.0f) Ang -= 360.0f; else if(Ang < -180.0f) Ang += 360.0f;

	return Ang;
}
