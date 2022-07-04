#include "WidgetZoneSelectButton.h"

#include "Components/ScrollBoxSlot.h"
#include "MyJrpg/DataTables/GatherTable.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UWidgetZoneSelectButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnEnter->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::MoveToZone);

	m_BtnClose->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnClose);

	m_BarSkull->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	m_BarWood->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_BarRock->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_BarItem->SetVisibility(ESlateVisibility::HitTestInvisible);

	m_BtnRun->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnRun);

	m_BtnWalk->OnClicked.AddDynamic(this,&UWidgetZoneSelectButton::OnWalk);

	UMyGameInstance::Get->m_ZoneMove->m_OnMoveEnd.AddUObject(this, &UWidgetZoneSelectButton::UpdateBtnText);
}

void UWidgetZoneSelectButton::SetBarGauge(UProgressBar* bar, int amount)
{
	float Value = (amount * 0.25f) + 0.04f;

	Value = FMath::Clamp(Value, 0.f, 1.f);

	bar->SetPercent(Value);
}

void UWidgetZoneSelectButton::SetPlayerHome()
{
	SetZone();

	m_BarSkull->GetParent()->SetVisibility(ESlateVisibility::Collapsed);
	
	m_BarWood->GetParent()->SetVisibility(ESlateVisibility::Collapsed);

	m_BarRock->GetParent()->SetVisibility(ESlateVisibility::Collapsed);

	m_BarItem->GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetZoneSelectButton::Init(const FZoneDataRow& zone_data)
{
	m_ZoneData = &zone_data;

	if(m_ZoneData->m_RowKey == TEXT("PlayerHome"))
	{
		SetPlayerHome();
		UpdateBtnText();
		return;
	}

	SetZone();

	SetItemsInSet();
	
	CreateZoneElement();

	SetBarGauge(m_BarSkull, m_ZoneData->m_nLevelMob);
	
	SetBarGauge(m_BarWood, m_ZoneData->m_nLevelWood);
	
	SetBarGauge(m_BarRock, m_ZoneData->m_nLevelRock);
	
	SetBarGauge(m_BarItem, m_ZoneData->m_nLevelItem);

	UpdateBtnText();
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
}

void UWidgetZoneSelectButton::MoveToZone()
{
	UMyGameInstance::Get->m_LevelMoveManager->OpenMyLevel(*m_ZoneData);
}

void UWidgetZoneSelectButton::OnWalk()
{
	if(UMyGameInstance::Get->m_ZoneMove->IsMoving())
	{
		return;
	}
	UMyGameInstance::Get->m_ZoneMove->StartMove(false, m_fWalkTime, m_ZoneData->m_RowKey);
	OnClose();
}

void UWidgetZoneSelectButton::OnRun()
{
	if(UMyGameInstance::Get->m_ZoneMove->IsMoving())
	{
		return;
	}
	UMyGameInstance::Get->m_ZoneMove->StartMove(true, m_fRunTime, m_ZoneData->m_RowKey);
	OnClose();
}

void UWidgetZoneSelectButton::GetRunStaminaCostTime(int& staminaCost, float& timeSpan)
{
	staminaCost = m_fDist / 30.f; 

	timeSpan = 1.f;
}

void UWidgetZoneSelectButton::GetWalkTime(float& timeSpan)
{
	timeSpan = m_fDist * 3.5f;
}

void UWidgetZoneSelectButton::UpdateBtnText()
{
	if(UMyGameInstance::Get->m_ZoneMove->IsZoneAlreadyIn(m_ZoneData->m_RowKey))
	{
		m_BtnEnter->SetVisibility(ESlateVisibility::Visible);
		
		m_BtnRun->SetVisibility(ESlateVisibility::Collapsed);

		m_BtnWalk->SetVisibility(ESlateVisibility::Collapsed);

		m_TextWalkTimeSpan->SetVisibility(ESlateVisibility::Collapsed);

		m_TextRunTimeSpan->SetVisibility(ESlateVisibility::Collapsed);

		//들어가기 버튼이 있어야함
		return;
	}
	
	m_fDist = UMyGameInstance::Get->m_ZoneMove->GetDist(m_ZoneData->m_RowKey);

	m_BtnEnter->SetVisibility(ESlateVisibility::Collapsed);
	
	m_BtnRun->SetVisibility(ESlateVisibility::Visible);

	m_BtnWalk->SetVisibility(ESlateVisibility::Visible);

	m_TextWalkTimeSpan->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_TextRunTimeSpan->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	GetRunStaminaCostTime(m_RunCost, m_fRunTime);

	GetWalkTime(m_fWalkTime);

	m_TextRunCost->SetText(FText::AsNumber(m_RunCost));

	const FString& CultName = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	
	FCulturePtr Culture = FInternationalization::Get().GetCulture(CultName);
	
	FTimespan Run(0,0,m_fRunTime);
	
	m_TextRunTimeSpan->SetText(FText::AsTimespan(Run, Culture));

	FTimespan Walk(0,0,m_fWalkTime);
	
	m_TextWalkTimeSpan->SetText(FText::AsTimespan(Walk, Culture));
}