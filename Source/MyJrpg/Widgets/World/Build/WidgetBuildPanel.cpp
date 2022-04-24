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

		m_ScrollElements->AddChild(SelectButton);
	}
}

void UWidgetBuildPanel::OnClickElement(UWidgetBuildElement* ele, const FBuildDataRow& data)
{
	if(m_Focused)
		m_Focused->MyUnFocus();
	m_Focused = ele;
	m_Focused->MyFocus();

	m_SelectedBuildData = &data;

	FVector Loc = UMyLib::GetPlayer()->GetActorLocation();
	
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(Loc, *m_SelectedBuildData);
}

void UWidgetBuildPanel::OnTouchWorld(const FHitResult& hit)
{
	if(!m_SelectedBuildData)
		return;
	
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(hit.Location, *m_SelectedBuildData);
}

void UWidgetBuildPanel::OpenPanel()
{
	Super::OpenPanel();

	UMyLib::GetPlayerCon()->EnableJoystick(false);
	
	m_Dele = UMyLib::GetPlayerCon()->m_OnTouch.AddUObject(this, &UWidgetBuildPanel::OnTouchWorld);

	UMyGameInstance::Get->m_BuildManager->StartBuilding();
}

void UWidgetBuildPanel::ClosePanel()
{
	Super::ClosePanel();

	UMyLib::GetPlayerCon()->EnableJoystick(true);

	UMyLib::GetPlayerCon()->m_OnTouch.Remove(m_Dele);

	if(m_Focused)
		m_Focused->MyUnFocus();

	m_Focused = nullptr;

	m_SelectedBuildData = nullptr;

	UMyGameInstance::Get->m_BuildManager->EndBuilding();

	
}
