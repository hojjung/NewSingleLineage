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
	UMyGameInstance::Get->m_BuildManager->Erase(Cast<IBuildable>(m_Owner.GetObject()));
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
	m_BtnCancel->SetVisibility(ESlateVisibility::Visible);
	if(b)
	{
		m_BtnConfirm->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_BtnConfirm->SetVisibility(ESlateVisibility::Collapsed);
	}
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
	UMyGameInstance::Get->m_BuildManager->Upgrade( Cast<IBuildable>(m_Owner.GetObject()));
}

void UWorldWidgetStruct::ShowSelect(bool b)
{
	DeselectErase();
	if(b)
	{
		m_BtnErase->SetVisibility(ESlateVisibility::Visible);
		bool Result = Cast<IBuildable>(m_Owner.GetObject())->HasNextUpgrade();//지맘대로네
		if(Result)
			m_BtnUpgrade->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_BtnUpgrade->SetVisibility(ESlateVisibility::Collapsed);
		m_BtnErase->SetVisibility(ESlateVisibility::Collapsed);
	}
	ShowBuildWidget(!b);
	ShowRotation(!b);
	m_BtnCancel->SetVisibility(ESlateVisibility::Collapsed);
}

void UWorldWidgetStruct::SetOwnerActor(AActor* actor)
{
	m_Owner.SetObject(actor);
	m_Owner.SetInterface(actor);
}