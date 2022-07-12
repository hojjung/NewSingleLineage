#include "EventStageManager.h"

#include "MyJrpg/DataTables/EventStageTable.h"

void UEventStageManager::Init()
{
	m_AryEventInst.Reserve(20);

	TArray<const FEventStageRow*> AryEvents;

	UEventStageTable::GetEventStage->GetAllRows("", AryEvents);

	for(const FEventStageRow* EventRow : AryEvents)
	{
		m_AryEventInst.Add(FEventStageSpec(*EventRow, this));
	}
}

void UEventStageManager::UpdateEvent()
{
	for(FEventStageSpec& EventBase : m_AryEventInst)
	{
		if(!EventBase.m_bIsUnlocked && EventBase.m_Cond->TryUnlockEvent())
		{
			EventBase.m_bIsUnlocked = true;

			EventBase.m_fDuration = EventBase.m_EventDataRow->m_fDuration;
			
			m_OnEventUnlocked.Broadcast(EventBase);
		}
	}
}

void UEventStageManager::Tick(float deltaTime)
{
	for(FEventStageSpec& EventBase : m_AryEventInst)
	{
		if(!EventBase.m_bIsUnlocked)
		{
			continue;
		}

		EventBase.m_fDuration -= deltaTime;

		m_OnEventTick.Broadcast(EventBase.m_fDuration);

		if(EventBase.m_fDuration <= 0.f)
		{
			EventBase.m_bIsUnlocked = false;

			EventBase.m_fDuration = 0.f;

			m_OnEventLocked.Broadcast(EventBase);
		}
	}
}
