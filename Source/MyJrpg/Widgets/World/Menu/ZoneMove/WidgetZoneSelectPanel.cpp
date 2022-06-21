#include "WidgetZoneSelectPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ZoneData.h"

void UWidgetZoneSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_CrntScroll = nullptr;

	m_BtnWorldMap->OnClicked.AddDynamic(this, &UWidgetZoneSelectPanel::OnClickWorldBtn);

	m_BtnSpecialMap->OnClicked.AddDynamic(this, &UWidgetZoneSelectPanel::OnClickSpecialBtn);

	m_BtnBossMap->OnClicked.AddDynamic(this, &UWidgetZoneSelectPanel::OnClickBossBtn);

	CreateButtons();

	OnClickWorldBtn();
}

void UWidgetZoneSelectPanel::CreateButtons()
{
	check(m_ClassButton);
	
	TArray<FZoneDataRow*> AryZoneDataRow;

	UZoneData::GetZoneTable->GetAllRows<FZoneDataRow>(TEXT(""),AryZoneDataRow);

	int I = 0;
	
	for(const FZoneDataRow* ZoneData : AryZoneDataRow)
	{
		UWidgetZoneSelectButton* SelectButton = CreateWidget<UWidgetZoneSelectButton>(this,m_ClassButton);

		SelectButton->Init(*ZoneData);

		I++;
	}

	m_ScrollWorld->SetVisibility(ESlateVisibility::Collapsed);
 
	m_ScrollSpecial->SetVisibility(ESlateVisibility::Collapsed);

	m_ScrollBoss->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetZoneSelectPanel::SetCrntZoneScrollVisible(UScrollBox* scroll)
{
	if(m_CrntScroll == scroll)
	{
		return;
	}
	
	if(m_CrntScroll)
	{
		m_CrntScroll->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	m_CrntScroll = scroll;	

	m_CrntScroll->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetZoneSelectPanel::OnClickWorldBtn()
{
	SetCrntZoneScrollVisible(m_ScrollWorld);
}

void UWidgetZoneSelectPanel::OnClickSpecialBtn()
{
	SetCrntZoneScrollVisible(m_ScrollSpecial);
}

void UWidgetZoneSelectPanel::OnClickBossBtn()
{
	SetCrntZoneScrollVisible(m_ScrollBoss);
}


