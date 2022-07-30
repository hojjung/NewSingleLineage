#include "ZoneMoveManager.h"

#include "MyGameInstance.h"

void UZoneMoveManager::Init()
{
	m_nZoneStamina  = 100;

	m_nRideEnergy  = 100;
	
	m_CurrentID = TEXT("PlayerHome");

	m_fRechargeTime = 10.f;
}

void UZoneMoveManager::ClearWidgetMap()
{
	m_MapZoneBtns.Reset();
}

void UZoneMoveManager::AddMapBtn(const FName& zoneID, UWidgetMapBtn* mapBtn)
{
	m_MapZoneBtns.Emplace(zoneID, TWeakObjectPtr<UWidgetMapBtn>(mapBtn));
}

void UZoneMoveManager::RemoveMapBtn(const FName& zoneID)
{
	TWeakObjectPtr<UWidgetMapBtn> Btn = m_MapZoneBtns.FindAndRemoveChecked(zoneID);

	Btn->SetVisibility(ESlateVisibility::Collapsed);

	Btn->RemoveFromParent();
}

void UZoneMoveManager::ZoneMoveDone(const FName& zoneID)
{
	if(!m_PendingKillID.IsNone())
	{
		RemoveMapBtn(m_PendingKillID);
		
		m_PendingKillID = NAME_None;
	}
	m_CurrentID = zoneID;
		
	m_DestZoneID = NAME_None;

	if(m_CurrentID == TEXT("PlayerHome"))
	{
		UMyGameInstance::Get->m_ZoneInst->ResetZone();
	}
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
		ZoneMoveDone(m_DestZoneID);
		
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

	m_OnStaminaChanged.Broadcast();

	return true;
}

bool UZoneMoveManager::TryPurchaseRideCost(int want)
{
	if(m_nRideEnergy < want)
	{
		return false;
	}
	
	m_nRideEnergy -= want;

	m_RiderEnergyInven->RemoveItem(0,want/5);
	
	m_OnRideCostChanged.Broadcast();

	return true;
}

void UZoneMoveManager::StartMove(bool isRunning, float timeUse, const FName& destZoneID)
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

float UZoneMoveManager::GetDist(const FName& dst)
{
	FVector2D SrcPos = m_MapZoneBtns[m_CurrentID]->GetPos();
	
	FVector2D DstPos = m_MapZoneBtns[dst]->GetPos();
	
	float Dist = FVector2D::Distance(DstPos, SrcPos);

	return Dist;
}

float UZoneMoveManager::GetDist(FVector2D loc)
{
	FVector2D SrcPos = loc;
	
	FVector2D DstPos = m_MapZoneBtns[m_DestZoneID]->GetPos();
	
	float Dist = FVector2D::Distance(DstPos, SrcPos);

	return Dist;
}

bool UZoneMoveManager::IsZoneAlreadyIn(const FName& dst)
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

	if(m_DestZoneID.IsNone())
	{
		return SrcPos;
	}
	FVector2D DstPos = m_MapZoneBtns[m_DestZoneID]->GetPos();

	float Percent = GetMovePercent();
	
	FVector2D DirPos = (DstPos - SrcPos) * Percent; 
		
	return SrcPos + DirPos;
}

FVector2D UZoneMoveManager::GetDestPos()
{
	FVector2D DstPos = m_MapZoneBtns[m_DestZoneID]->GetPos();

	return DstPos;
}

float UZoneMoveManager::GetEulerAngle(const FName& dst)
{
	FVector2D SrcPos = m_MapZoneBtns[m_CurrentID]->GetPos();
	
	FVector2D DstPos = m_MapZoneBtns[dst]->GetPos();

	FVector Start = FVector(SrcPos.X, SrcPos.Y, 0.0f);
	FVector Dest = FVector(DstPos.X, DstPos.Y, 0.0f);
	FVector Dir = Dest - Start;
	FVector GoalDirection = Dir.GetSafeNormal();
	//노말라이징한 두개의 백터를 dot한다.
	////여기서 축을 Z축으로 하기 위해 두백터의 Z값을 0.0f로 넣어 주었다.
	float dot = FVector::DotProduct(FVector::ForwardVector, GoalDirection);
	float AcosAngle = FMath::Acos(dot);
	// dot한 값을 아크코사인 계산해 주면 0 ~ 180도 사이의 값 (0 ~ 1)의 양수 값만 나온다.
	float angle = FMath::RadiansToDegrees(AcosAngle);
	//그값은 degrees 값인데 이것에 1라디안을 곱해주면 60분법의 도가 나온다.
	////여기서 두 백터를 크로스 하여 회전할 축을 얻게 된다.
	/////이 크로스 백터는 Axis회전의 회전축이 되며 , 그 양수 음수로 회전 방향 왼쪽(음수), 오른쪽(양수)를 알수 있다.
	FVector cross = FVector::CrossProduct(FVector::ForwardVector, GoalDirection);

	if (cross.Z < 0)
	{
		angle = -angle;
	}
		
	return angle;
}

bool UZoneMoveManager::IsMoving()
{
	return !m_DestZoneID.IsNone();
}

bool UZoneMoveManager::IsRunning()
{
	return m_bIsRunning;
}

float UZoneMoveManager::GetRemainTime()
{
	return m_fMaxDuration - m_fRemainDuration;
}

void UZoneMoveManager::GetRunStaminaCostTime(const float& distIn, int& outRunCost, float& outRunTime)
{
	outRunCost = distIn / 25.f;
	outRunCost = FMath::Min(outRunCost, 50);

	outRunTime = distIn / 30.f;
}

void UZoneMoveManager::GetRideStaminaCostTime(const float& distIn, int& outRunCost, float& outRunTime)
{
	outRunCost = distIn / 13.f;
	
	outRunCost = FMath::Min(outRunCost, 90);

	outRunTime = distIn / 30.f;
}

float UZoneMoveManager::GetWalkTime(const float& distIn)
{
	float RunTime = distIn * 3.5f;
	
	RunTime = FMath::Min(RunTime, 3600.f);
	
	return RunTime;
}

FName UZoneMoveManager::GetDestZoneID()
{
	return m_DestZoneID;
}

FName UZoneMoveManager::GetCurrentZoneID()
{
	return m_CurrentID;
}

int UZoneMoveManager::GetStamina()
{
	return m_nZoneStamina;
}

int UZoneMoveManager::GetRideEnergy()
{
	return m_nRideEnergy;
}

float UZoneMoveManager::GetStaminaChargeTime() const
{
	return m_fRechargeTime;
}

UWidgetMapBtn* UZoneMoveManager::GetMapBtn(const FName& zoneID)
{
	return m_MapZoneBtns[zoneID].Get();
}

void UZoneMoveManager::SetRiderEnergy(int v)
{
	m_nRideEnergy = v;

	m_OnRideCostChanged.Broadcast();
}

void UZoneMoveManager::SetRiderEnergyInven(UInventory* inventory)
{
	m_RiderEnergyInven = inventory;
}

void UZoneMoveManager::SetPendingKillStage(const FName& id)
{
	m_PendingKillID = id;
}