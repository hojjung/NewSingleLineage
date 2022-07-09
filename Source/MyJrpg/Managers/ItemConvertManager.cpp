#include "ItemConvertManager.h"

void UItemConvertManager::AddStructureAndItem(AStructureActor* actor, UItemConvertInst* inst)
{
	m_MapItemConverters.Add(actor,inst);
}

void UItemConvertManager::RemoveStructureAndItem(AStructureActor* actor)
{
	m_MapItemConverters.Remove(actor);
}

void UItemConvertManager::Tick(float delta_time)
{
	for(auto& ItemConvertPair : m_MapItemConverters)
	{
		ItemConvertPair.Value->Tick(delta_time);
	}
}
