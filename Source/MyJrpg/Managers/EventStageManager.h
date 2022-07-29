// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/EventStageTable.h"
#include "MyJrpg/EventConditions/EventConditionBase.h"
#include "UObject/NoExportTypes.h"
#include "EventStageManager.generated.h"

/**
 * 
 */

USTRUCT()
struct FEventStageSpec
{
	GENERATED_BODY()

public:
	FEventStageSpec(): m_EventDataRow(nullptr), m_Cond(nullptr), m_bIsUnlocked(false), m_fDuration(0)
	{
		
	}

	FEventStageSpec(const FEventStageRow& dataRow, UObject* outer)
	{
		m_EventDataRow = &dataRow;
		
		m_bIsUnlocked = false;

		m_fDuration = 0.f;

		m_Cond = NewObject<UEventConditionBase>(outer, m_EventDataRow->m_ClassCondition);
	}

public:
	const FEventStageRow* m_EventDataRow;
	
	UPROPERTY()
	UEventConditionBase* m_Cond;

	bool m_bIsUnlocked;

	float m_fDuration;
};

UCLASS()
class MYJRPG_API UEventStageManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventUnlocked, const FEventStageSpec&);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEventTick, float);

	FOnEventUnlocked m_OnEventUnlocked;

	FOnEventUnlocked m_OnEventLocked;

	FOnEventTick m_OnEventTick;

protected:
	UPROPERTY()
	TArray<FEventStageSpec> m_AryEventInst;

public:
	void Init();
	
	void UpdateEvent();

	void Tick(float deltaTime);

	FORCEINLINE const TArray<FEventStageSpec>& GetEvents() const
	{
		return m_AryEventInst;
	}

	bool IsTimeEnough(float t, FName zoneID);
};
