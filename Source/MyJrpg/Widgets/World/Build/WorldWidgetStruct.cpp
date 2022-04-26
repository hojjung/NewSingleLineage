#include "WorldWidgetStruct.h"

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Managers/ConstructionManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWorldWidgetStruct::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_OverlayErase->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCancel->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnCancel);

	m_BtnConfirm->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnConfirm);

	m_BtnRotation->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnRotation);

	m_BtnErase->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnErase);

	m_BtnUpgrade->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnUpgrade);

	ShowSelect(false);
}

void UWorldWidgetStruct::SelectErase()
{
	m_OverlayErase->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWorldWidgetStruct::DeselectErase()
{
	m_OverlayErase->SetVisibility(ESlateVisibility::Collapsed);
}

void UWorldWidgetStruct::ConfirmErase()
{
	UMyGameInstance::Get->m_BuildManager->Erase(m_Owner);
}

void UWorldWidgetStruct::ShowRotation(bool b)
{
	if(b)
		m_BtnRotation->SetVisibility(ESlateVisibility::Visible);
	else
		m_BtnRotation->SetVisibility(ESlateVisibility::Collapsed);
}

void UWorldWidgetStruct::ShowBuildWidget(bool b)
{
	if(b)
		m_BtnConfirm->SetVisibility(ESlateVisibility::Visible);
	else
		m_BtnConfirm->SetVisibility(ESlateVisibility::Collapsed);
}

void UWorldWidgetStruct::OnCancel()
{
	UMyGameInstance::Get->m_BuildManager->Cancel();
}

void UWorldWidgetStruct::OnConfirm()
{
	UMyGameInstance::Get->m_BuildManager->ConfirmBuild();
}

void UWorldWidgetStruct::OnRotation()
{
	UMyGameInstance::Get->m_BuildManager->Rotate();
}

void UWorldWidgetStruct::OnErase()
{
	if(m_OverlayErase->IsVisible())
	{
		ConfirmErase();
	}
	else
	{
		SelectErase();
	}
}

void UWorldWidgetStruct::OnUpgrade()
{
	UMyGameInstance::Get->m_BuildManager->Upgrade(m_Owner);
}

void UWorldWidgetStruct::ShowSelect(bool b)
{
	DeselectErase();
	if(b)
	{
		m_BtnErase->SetVisibility(ESlateVisibility::Visible);
		
		m_BtnCancel->SetVisibility(ESlateVisibility::Collapsed);
		
		if(m_Owner->HasNextUpgrade())
			m_BtnUpgrade->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_BtnUpgrade->SetVisibility(ESlateVisibility::Collapsed);
		m_BtnErase->SetVisibility(ESlateVisibility::Collapsed);
		m_BtnCancel->SetVisibility(ESlateVisibility::Visible);
	}
	ShowBuildWidget(!b);
	ShowRotation(!b);
}

void UWorldWidgetStruct::SetOwnerActor(AStructureActor* actor)
{
	m_Owner = actor;
}