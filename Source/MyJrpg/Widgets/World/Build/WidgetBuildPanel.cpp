#include "WidgetBuildPanel.h"
#include "MyJrpg/Managers/ConstructionManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetBuildPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const TArray<FBuildDataRow*>& AryDatas = UMyGameInstance::Get->m_BuildManager->GetAryBuildDatas();

	for(const FBuildDataRow* Data : AryDatas)
	{
		UWidgetBuildElement* SelectButton = CreateWidget<UWidgetBuildElement>(this,m_ClassBuildEle);

		SelectButton->Init(*Data);
		SelectButton->m_OnClick.BindUObject(this, &UWidgetBuildPanel::OnClickElement);
	}
}

void UWidgetBuildPanel::OnClickElement(UWidgetBuildElement* ele, const FBuildDataRow& data)
{
	if(m_Focused)
		m_Focused->MyUnFocus();
	m_Focused = ele;
	m_Focused->MyFocus();
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(data);
}

void UWidgetBuildPanel::OpenPanel()
{
	Super::OpenPanel();
}

void UWidgetBuildPanel::ClosePanel()
{
	Super::ClosePanel();

	m_Focused = nullptr;
}
