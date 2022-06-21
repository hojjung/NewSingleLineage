#include "WidgetZoneSelectButton.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UWidgetZoneSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnEnterZone->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::MoveToZone);

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnClose);
}

void UWidgetZoneSelectButton::Init(const FZoneDataRow& zone_data)
{
	m_ZoneData = &zone_data;

	SetZone();
}

void UWidgetZoneSelectButton::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);	
}

void UWidgetZoneSelectButton::CreateMonsters()
{
	m_HoriMonsterParents->ClearChildren();

	m_SetMonsters.Reset();
	
	if(!m_ZoneData->m_SpawnDataNpc)
	{
		return;
	}
	
	for(const FNPCSpawnData& Data : m_ZoneData->m_SpawnDataNpc->m_ArySpawnDatas)
	{
		if(Data.m_EntityParentTable->RowStruct->IsChildOf(FNpcUnitEntityRow::StaticStruct()))
		{
			const FNpcUnitEntityRow* NpcEntity = Data.m_EntityParentTable->FindRow<FNpcUnitEntityRow>(Data.m_IDEntity, "");
	
			m_SetMonsters.Add(NpcEntity);			
		}
	}
	
	m_SetMonsters.Sort([](const FNpcUnitEntityRow& LHS, const FNpcUnitEntityRow& RHS)  { return LHS.m_fExp > RHS.m_fExp; });
	
	for(const FNpcUnitEntityRow* Unit : m_SetMonsters)
	{
		UWidgetZoneMonsterElement* SelectButton = CreateWidget<UWidgetZoneMonsterElement>(this,m_ClassMonster);
	
		SelectButton->SetUnit(Unit);
		//출현 몬스터
		m_HoriMonsterParents->AddChild(SelectButton);
	}
}

void UWidgetZoneSelectButton::CreateZoneElement(const TArray<FDropRewardItem>& AryItems)
{
	for(const FDropRewardItem& Data : AryItems)
	{
		bool AlreadyAdd = false;
		
		m_SetRewardItems.Add(Data.m_Item.RowName, &AlreadyAdd);

		if(AlreadyAdd)
		{
			continue;
		}
		
		UWidgetZoneItemElement* SelectButton = CreateWidget<UWidgetZoneItemElement>(this,m_ClassItem);

		SelectButton->SetZone(Data);

		m_AryZoneElements.Add(SelectButton);
	}
}

void UWidgetZoneSelectButton::CreateItems()
{
	m_HoriItemParents->ClearChildren();

	m_AryZoneElements.Reset();

	m_SetRewardItems.Reset();
	
	for(const FNpcUnitEntityRow* Unit : m_SetMonsters)
	{
		CreateZoneElement(Unit->m_AryDropItem);
	}

	m_AryZoneElements.Sort([](const UWidgetZoneItemElement& ll, const UWidgetZoneItemElement& rr){return ll.GetSortValue() > rr.GetSortValue();});

	for(UWidgetZoneItemElement* Ele : m_AryZoneElements)
	{
		m_HoriItemParents->AddChild(Ele);
	}
}

void UWidgetZoneSelectButton::SetZone()
{
	m_TextMapName->SetText(m_ZoneData->m_ShowingName);

	m_TextMapDesc->SetText(m_ZoneData->m_Desc);

	CreateMonsters();
	
	CreateItems();
}

void UWidgetZoneSelectButton::MoveToZone()
{
	UMyGameInstance::Get->m_LevelMoveManager->OpenMyLevel(*m_ZoneData);
}