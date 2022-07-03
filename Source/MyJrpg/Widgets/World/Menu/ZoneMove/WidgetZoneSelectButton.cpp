#include "WidgetZoneSelectButton.h"

#include "Components/ScrollBoxSlot.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UWidgetZoneSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnEnterZone->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::MoveToZone);

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnClose);

	m_BarSkull->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	m_BarWood->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_BarRock->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_BarItem->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWidgetZoneSelectButton::SetBarGauge(UProgressBar* bar, int amount)
{
	float Value = (amount * 0.25f) + 0.04f;

	Value = FMath::Clamp(Value, 0.f, 1.f);

	bar->SetPercent(Value);
}

void UWidgetZoneSelectButton::Init(const FZoneDataRow& zone_data)
{
	m_ZoneData = &zone_data;

	SetZone();

	SetBarGauge(m_BarSkull, m_ZoneData->m_nLevelMob);
	SetBarGauge(m_BarWood, m_ZoneData->m_nLevelWood);
	SetBarGauge(m_BarRock, m_ZoneData->m_nLevelRock);
	SetBarGauge(m_BarItem, m_ZoneData->m_nLevelItem);
}

void UWidgetZoneSelectButton::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);	
}

void UWidgetZoneSelectButton::SetItemsInSet()
{
	m_SetRewardItems.Reset();
	
	if(!m_ZoneData->m_SpawnDataNpc)
	{
		return;
	}
	
	for(const FNPCSpawnData& Data : m_ZoneData->m_SpawnDataNpc->m_ArySpawnDatas)
	{
		if(Data.m_EntityParentTable->RowStruct->IsChildOf(FNpcUnitEntityRow::StaticStruct()))
		{
			const FNpcUnitEntityRow* NpcEntity = Data.m_EntityParentTable->FindRow<FNpcUnitEntityRow>(Data.m_IDEntity, "");

			for(const FDropRewardItem& DropItem : NpcEntity->m_AryDropItem)
			{
				m_SetRewardItems.Add(DropItem.m_Item.RowName);			
			}
		}
		else if(Data.m_EntityParentTable->RowStruct->IsChildOf(FGatherDataRow::StaticStruct()))
		{
			const FGatherDataRow* GatherEntity = Data.m_EntityParentTable->FindRow<FGatherDataRow>(Data.m_IDEntity, "");

			m_SetRewardItems.Add(GatherEntity->m_ItemGather.RowName);
		}
		else if(Data.m_EntityParentTable->RowStruct->IsChildOf(FBuildDataRow::StaticStruct()))
		{
			const FBuildDataRow* BuildEntity = Data.m_EntityParentTable->FindRow<FBuildDataRow>(Data.m_IDEntity, "");

			if(!BuildEntity->m_ClassInter->IsChildOf(UBI_StorageSearch::StaticClass()))
			{
				continue;
			}
			const TArray<FString>& Items = BuildEntity->m_AryInteractVariable;
			if(Items.Num() % 2 != 0 && Items.Num() >= 3)
			{
				int Iter = 1;
				while (Iter < Items.Num())
				{
					FName ItemID = *Items[Iter];

					m_SetRewardItems.Add(ItemID);
					
					Iter+=2;
				}
			}
		}
	}
	
	m_SetRewardItems.Sort([](const FName& LHS, const FName& RHS)  { return LHS.FastLess(RHS); });
}

void UWidgetZoneSelectButton::CreateZoneElement()
{
	m_HoriItemParents->ClearChildren();
	
	for(const FName& Data : m_SetRewardItems)
	{
		UWidgetZoneItemElement* SelectButton = CreateWidget<UWidgetZoneItemElement>(this,m_ClassItem);

		SelectButton->SetZone(Data);

		UPanelSlot* PanelSlotWant = m_HoriItemParents->AddChild(SelectButton);

		UScrollBoxSlot* ScrollPanelSlot = Cast<UScrollBoxSlot>(PanelSlotWant);
		
		ScrollPanelSlot->SetPadding(FMargin(0,30,0,0));
	}
}

void UWidgetZoneSelectButton::SetZone()
{
	m_TextMapName->SetText(m_ZoneData->m_ShowingName);

	m_TextMapDesc->SetText(m_ZoneData->m_Desc);

	SetItemsInSet();
	
	CreateZoneElement();
}

void UWidgetZoneSelectButton::MoveToZone()
{
	UMyGameInstance::Get->m_LevelMoveManager->OpenMyLevel(*m_ZoneData);
}