#include "AssembleInst.h"

#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UAssembleInst::SetCraftItem(const FCraftable& craftable)
{
	m_Craftable = &craftable;

	Init( FMath::Min(m_Craftable->m_AryCostItem.Num(), 10), m_Craftable->m_ShowingName);
}

bool UAssembleInst::IsSlotPutable(int index, const FItemSpec& item)//들어가려면
{
	const FCraftItemCost& CraftItem = m_Craftable->m_AryCostItem[index];

	const FItemSpec& CurrentItem = m_AryTotalItems[index];

	return CraftItem.m_ItemDataRowHandle.RowName == item.m_ID && CraftItem.m_nStackOrLevel > CurrentItem.m_nLvStack;//더 들어가려면 비워줘야한다
}

bool UAssembleInst::IsSlotMatch(int index)
{
	const FCraftItemCost& CraftItem = m_Craftable->m_AryCostItem[index];

	const FItemSpec& CurrentItem = m_AryTotalItems[index];

	return CraftItem.m_ItemDataRowHandle.RowName == CurrentItem.m_ID && CraftItem.m_nStackOrLevel == CurrentItem.m_nLvStack;//더 들어가려면 비워줘야한다
}

const FCraftable& UAssembleInst::GetCraftData() const
{
	return *m_Craftable;
}

bool UAssembleInst::TryComplete()
{
	int Index = 0;
	
	for(const FItemSpec& ItemIn : m_AryTotalItems)
	{
		if(!IsSlotMatch(Index))
		{
			return false;
		}
		Index++;
	}
	return UMyGameInstance::Get->m_BuildManager->Upgrade(Cast<AStructureActor>(GetOuter()));
}

void UAssembleInst::PutItem(int index, UInventory* from)
{
	
}
