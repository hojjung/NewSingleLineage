#include "WidgetZoneSelectPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/ZoneData.h"

void UWidgetZoneSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnWorldMap->IsFocusable = false;
	
	m_BtnSpecialMap->IsFocusable = false;

	m_BtnBossMap->IsFocusable = false;

	CreateButtons();
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

		m_ScrollWorldButton->AddChild(SelectButton);

		I++;
	}
}


