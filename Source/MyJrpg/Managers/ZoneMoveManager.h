// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Actors/Field/Build/BuildInteract/BI_Horse.h"
#include "MyJrpg/Widgets/MapSelect/WidgetMapBtn.h"
#include "MyJrpg/Widgets/World/Menu/ZoneMove/WidgetZoneSelectButton.h"
#include "UObject/NoExportTypes.h"
#include "ZoneMoveManager.generated.h"

/**
 *  스태미나 충전 및 소모
 *  달리기 코스트 계산
 *  실제 달리기 타임 계산
 */
UCLASS()
class MYJRPG_API UZoneMoveManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnStamina);
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FMoveStart, const FName& , float);

	DECLARE_MULTICAST_DELEGATE(FOnMoveTick);

	FMoveStart m_OnMoveStart;

	FOnMoveTick m_OnMoveTick;

	FOnMoveTick m_OnMoveEnd;

	FOnStamina m_OnStaminaChanged;

	FOnStamina m_OnRideCostChanged;

protected:
	int m_nRideEnergy;
	
	int m_nZoneStamina;

	float m_fRemainDuration;

	float m_fMaxDuration;

	float m_fMaxDistance;

	bool m_bIsRunning;

	FName m_DestZoneID;

	FName m_CurrentID;//현재 위치한 존

	TMap<FName,TWeakObjectPtr<UWidgetMapBtn>> m_MapZoneBtns;

	float m_fRechargeTime;

	UPROPERTY()
	UInventory* m_RiderEnergyInven;
	
public:
	void Init();

	void ClearWidgetMap();

	void AddMapBtn(FName zoneID, UWidgetMapBtn* mapBtn);

	void RemoveMapBtn(FName zoneID);
	
	void ZoneMoveDone(FName zoneID);

	void Tick(float deltaTime);

	bool TryPurchaseStamina(int want);

	bool TryPurchaseRideCost(int want);
	
	void StartMove(bool isRunning, float timeUse, FName destZoneID);

	float GetMovePercent();

	float GetDist(FName dst);

	float GetDist(FVector2D loc);
	
	bool IsZoneAlreadyIn(FName dst);
	
	FVector2D GetBarPos(const FName& dst);

	FVector2D GetPlayerIconPos();

	FVector2D GetDestPos();
	
	float GetEulerAngle(const FName& dst);

	bool IsMoving();

	bool IsRunning();

	float GetRemainTime();

	void GetRunStaminaCostTime(const float& distIn, int& outRunCost, float& outRunTime);

	void GetRideStaminaCostTime(const float& distIn, int& outRunCost, float& outRunTime);

	float GetWalkTime(const float& distIn);

	FName GetDestZoneID();

	int GetStamina();

	int GetRideEnergy();

	float GetStaminaChargeTime() const;

	UWidgetMapBtn* GetMapBtn(FName zoneID);

	void SetRiderEnergy(int v);
	
	void SetRiderEnergyInven(UInventory* inventory);
};
