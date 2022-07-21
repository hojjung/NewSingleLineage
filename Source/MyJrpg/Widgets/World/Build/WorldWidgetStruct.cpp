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
	UMyGameInstance::Get->m_BuildManager->Erase(m_Owner.Get(), true, false);
}

void UWorldWidgetStruct::UpdateUpgradeCost()
{
	const FBuildDataRow& NextBuildData = *UBuildData::GetBuildTable->FindRow<FBuildDataRow>(m_Owner->GetBuildData().m_NextUpgradeActorID, "");

	CreateCostWidgets(NextBuildData.m_AryCostItem);
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
	UMyGameInstance::Get->m_BuildManager->Upgrade( m_Owner.Get());
}

void UWorldWidgetStruct::ShowSelect(bool b)
{
	DeselectErase();
	if(b)
	{
		m_BtnErase->SetVisibility(ESlateVisibility::Visible);
		if(m_Owner->HasUpgrade())
		{
			m_BtnUpgrade->SetVisibility(ESlateVisibility::Visible);
			m_VertCost->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UpdateUpgradeCost();
		}
	}
	else
	{
		m_BtnUpgrade->SetVisibility(ESlateVisibility::Collapsed);
		m_VertCost->SetVisibility(ESlateVisibility::Collapsed);
		m_BtnErase->SetVisibility(ESlateVisibility::Collapsed);
	}
	ShowBuildWidget(!b);
	ShowRotation(!b);
	m_BtnCancel->SetVisibility(ESlateVisibility::Collapsed);
}

void UWorldWidgetStruct::SetOwnerActor(AStructureActor* actor)
{
	m_Owner = actor;
}

void UWorldWidgetStruct::CreateCostWidgets(const TArray<FCraftItemCost>& costData)
{
	m_VertCost->ClearChildren();

	m_CraftCost.Reset();
	
	for(const FCraftItemCost& CraftData : costData)
	{
		UWidgetCraftCostElement* SelectButton = CreateWidget<UWidgetCraftCostElement>(this,m_ClassCostElement);

		SelectButton->BoundStackDefaultStackFunPtr();
		
		SelectButton->SetCraftCost(CraftData);

		m_VertCost->AddChildToVerticalBox(SelectButton);

		m_CraftCost.Add(SelectButton);
		
		SelectButton->SetPadding(FMargin(0,0,0,0));

		SelectButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
