#include "WidgetZoneMonsterElement.h"

void UWidgetZoneMonsterElement::SetUnit(const FNpcUnitEntityRow* npc_entity)
{
	//m_ElementBase->SetIcon(npc_entity->m_Icon);


	m_ElementBase->SetHoldable(false);

	//m_ElementBase->m_OnHold.AddUObject(this,&UWidgetZoneMonsterElement::OnHoldingComplete);
}

void UWidgetZoneMonsterElement::OnHoldingComplete()
{
	
}

