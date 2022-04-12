#include "WidgetZoneSelectPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ZoneData.h"

void UWidgetZoneSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PtrCrnt = nullptr;

	m_BtnWorldMap->OnClicked.AddDynamic(this, &UWidgetZoneSelectPanel::OnClickWorldBtn);

	m_BtnSpecialMap->OnClicked.AddDynamic(this, &UWidgetZoneSelectPanel::OnClickSpecialBtn);

	m_BtnBossMap->OnClicked.AddDynamic(this, &UWidgetZoneSelectPanel::OnClickBossBtn);

	CreateButtons();
}

void UWidgetZoneSelectPanel::CreateButtons()
{
	check(m_ClassButton);
	
	TArray<FZoneDataRow*> AryZoneDataRow;

	UZoneData::GetZoneTable->GetAllRows<FZoneDataRow>(TEXT(""),AryZoneDataRow);

	m_AryDefaultZoneBtns.Reset();

	m_ArySpecialZoneBtns.Reset();

	m_AryBossZoneBtns.Reset();

	m_ScrollWorldButton->ClearChildren();
	
	int I = 0;
	
	for(const FZoneDataRow* ZoneData : AryZoneDataRow)
	{
		UWidgetZoneSelectButton* SelectButton = CreateWidget<UWidgetZoneSelectButton>(this,m_ClassButton);

		SelectButton->Init(*ZoneData);

		SelectButton->SetVisibility(ESlateVisibility::Hidden);
		
		switch (ZoneData->m_MapType)
		{
		case EMapType::Default:
			m_AryDefaultZoneBtns.Add(SelectButton);
			break;
		case EMapType::Special:
			m_ArySpecialZoneBtns.Add(SelectButton);
			break;
		case EMapType::Boss:
			m_AryBossZoneBtns.Add(SelectButton);
			break;
		}
		m_ScrollWorldButton->AddChild(SelectButton);

		I++;
	}

	m_PtrCrnt = nullptr;

	OnClickWorldBtn();
}

void UWidgetZoneSelectPanel::SetCrntZoneAry(TArray<UWidgetZoneSelectButton*>& aryWant)
{
	if(m_PtrCrnt == &aryWant)
	{
		return;
	}
	
	if(m_PtrCrnt)
	{
		for(UWidgetZoneSelectButton* Btn : *m_PtrCrnt)
		{
			Btn->SetVisibility(ESlateVisibility::Collapsed);
		}	
	}
	
	for(UWidgetZoneSelectButton* Btn : aryWant)
	{
		Btn->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	m_PtrCrnt = &aryWant;	
}

void UWidgetZoneSelectPanel::OnClickWorldBtn()
{
	SetCrntZoneAry(m_AryDefaultZoneBtns);
}

void UWidgetZoneSelectPanel::OnClickSpecialBtn()
{
	SetCrntZoneAry(m_ArySpecialZoneBtns);
}

void UWidgetZoneSelectPanel::OnClickBossBtn()
{
	SetCrntZoneAry(m_AryBossZoneBtns);
}


