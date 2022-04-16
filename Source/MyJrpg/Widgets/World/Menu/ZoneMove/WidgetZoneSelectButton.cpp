#include "WidgetZoneSelectButton.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UWidgetZoneSelectButton::Init(const FZoneDataRow& zone_data)
{
	m_ZoneData = &zone_data;

	m_nIndex = 0;
	
	m_BtnEnterZone->IsFocusable = false;

	m_BtnEnterZone->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::MoveToZone);

	m_BtnLeftIndex->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnLeftClick);

	m_BtnRightIndex->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnRightClick);

	SetZone();

	if (m_ZoneData->m_AryZones.Num()<=1)
	{
		m_BtnLeftIndex->SetVisibility(ESlateVisibility::Collapsed);
		m_BtnRightIndex->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWidgetZoneSelectButton::UpdateText()
{
	if (m_ZoneData->m_AryZones.Num() > 1)
	{
		FString Str = FString::Printf(TEXT("%s %d층"),*m_ZoneData->m_ShowingName.ToString(),m_nIndex + 1);
		
		FText tLevelName = FText::FromString(Str);
		
		m_TextMapName->SetText(tLevelName);
	}
}

void UWidgetZoneSelectButton::CreateMonsters(const FZone& zone_data)
{
	m_HoriMonsterParents->ClearChildren();

	m_SetMonsters.Reset();
	
	if(!zone_data.m_SpawnDataNpc)
	{
		return;
	}
	
	for(const FNPCSpawnData& Data : zone_data.m_SpawnDataNpc->m_ArySpawnDatas)
	{
		const FNpcUnitEntityRow* NpcEntity = UUnitEntityData::GetNpcUnitTable->FindRow<FNpcUnitEntityRow>(Data.m_IDEntity, "");
	
		if(m_SetMonsters.Contains(NpcEntity))
		{
			continue;
		}
	
		m_SetMonsters.Add(NpcEntity);//소트?
	}
	
	m_SetMonsters.Sort([](const FNpcUnitEntityRow& LHS, const FNpcUnitEntityRow& RHS)  { return LHS.m_fExp > RHS.m_fExp; });
	
	for(const FNpcUnitEntityRow* Unit : m_SetMonsters)
	{
		UWidgetZoneMonsterElement* SelectButton = CreateWidget<UWidgetZoneMonsterElement>(this,m_ClassMonster);
	
		SelectButton->SetUnit(Unit);
		//출현 몬스터
		m_HoriMonsterParents->AddChildToHorizontalBox(SelectButton);
	}
}

void UWidgetZoneSelectButton::CreateZoneElement(const TArray<FDropRewardItem>& AryItems)
{
	for(const FDropRewardItem& Data : AryItems)
	{
		if(m_SetRewardItems.Contains(Data.m_Item.RowName))
		{
			return;
		}
		
		m_SetRewardItems.Add(Data.m_Item.RowName);
		
		UWidgetZoneItemElement* SelectButton = CreateWidget<UWidgetZoneItemElement>(this,m_ClassItem);

		SelectButton->SetZone(Data);

		m_AryZoneElements.Add(SelectButton);
	}
}

void UWidgetZoneSelectButton::CreateItems(const FZone& zone_data)
{
	m_HoriItemParents->ClearChildren();

	m_AryZoneElements.Reset();

	m_SetRewardItems.Reset();
	
	const TArray<FDropRewardItem>* AryDropItems = UMyGameInstance::Get->m_RewardManager->GetDropItems(zone_data.m_ZoneUniqueID);

	if(AryDropItems && AryDropItems->Num() > 0)
	{
		CreateZoneElement(*AryDropItems);	
	}
	
	for(const FNpcUnitEntityRow* Unit : m_SetMonsters)
	{
		CreateZoneElement(Unit->m_AryDropItem);
	}

	m_AryZoneElements.Sort([](const UWidgetZoneItemElement& ll, const UWidgetZoneItemElement& rr){return ll.GetSortValue() > rr.GetSortValue();});

	for(UWidgetZoneItemElement* Ele : m_AryZoneElements)
	{
		m_HoriItemParents->AddChildToHorizontalBox(Ele);
	}
}

void UWidgetZoneSelectButton::SetZone()
{
	m_TextMapName->SetText(m_ZoneData->m_ShowingName);

	m_TextMapDesc->SetText(m_ZoneData->m_Desc);

	CreateMonsters(m_ZoneData->m_AryZones[m_nIndex]);
	
	CreateItems(m_ZoneData->m_AryZones[m_nIndex]);

	UpdateText();
}

void UWidgetZoneSelectButton::MoveToZone()
{
	UMyGameInstance::Get->m_LevelMoveManager->OpenMyLevel(*m_ZoneData,m_nIndex);
}

void UWidgetZoneSelectButton::OnLeftClick()
{
	m_nIndex--;

	m_nIndex = FMath::Max(m_nIndex,0);

	SetZone();
}

void UWidgetZoneSelectButton::OnRightClick()
{
	m_nIndex++;
	
	m_nIndex = FMath::Min(m_nIndex,m_ZoneData->m_AryZones.Num() - 1);

	SetZone();
}
