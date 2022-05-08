#include "WidgetBuildPanel.h"

#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Managers/ConstructionManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetBuildPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const TArray<FBuildDataRow*>& AryDatas = UMyGameInstance::Get->m_BuildManager->GetAryBuildDatas();

	for(const FBuildDataRow* Data : AryDatas)
	{
		if(Data->m_BuildType == EBuildType::Furniture)
		{
			continue;
		}
		UWidgetBuildElement* SelectButton = CreateWidget<UWidgetBuildElement>(this,m_ClassBuildEle);
		SelectButton->Init(*Data);
		SelectButton->CreateCostWidgets(Data->m_AryCostItem);
		SelectButton->m_OnClick.BindUObject(this, &UWidgetBuildPanel::OnClickElement);
		m_ScrollElements->AddChild(SelectButton);
	}
	
	m_ScrollFurnitureElements->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnStruct->OnClicked.AddDynamic(this, &UWidgetBuildPanel::OnClickStruct);
	m_BtnFurniture->OnClicked.AddDynamic(this, &UWidgetBuildPanel::OnClickFurniture);
}

void UWidgetBuildPanel::OnClickElement(UWidgetBuildElement* ele, const FBuildDataRow& data)
{
	if(m_Focused)
		m_Focused->MyUnFocus();
	m_Focused = ele;
	m_Focused->MyFocus();

	FVector Loc = UMyLib::GetPlayer()->GetActorLocation();
	UMyGameInstance::Get->m_BuildManager->CancelSelect();
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(Loc, &data);
}

void UWidgetBuildPanel::OnTouchWorld(const FHitResult& hit)
{
	IBuildable* SActor = Cast<IBuildable>(hit.Actor.Get());
	
	if(SActor && !m_Focused)
	{
		UMyGameInstance::Get->m_BuildManager->Cancel();
		UMyGameInstance::Get->m_BuildManager->CancelSelect();
		UMyGameInstance::Get->m_BuildManager->SelectStruct(SActor);
		return;		
	}
	UMyGameInstance::Get->m_BuildManager->CancelSelect();
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(hit.Location);
}

void UWidgetBuildPanel::OpenPanel()
{
	Super::OpenPanel();

	UMyLib::GetPlayerCon()->EnableJoystick(false);
	
	UMyGameInstance::Get->m_BuildManager->StartBuilding();
	
	m_DeleTouchWorld = UMyLib::GetPlayerCon()->m_OnTouch.AddUObject(this, &UWidgetBuildPanel::OnTouchWorld);

	m_DeleCancel = UMyGameInstance::Get->m_BuildManager->m_OnCancel.AddUObject(this, &UWidgetBuildPanel::OnCancel);
	
	m_DeleFurniture = UMyGameInstance::Get->m_BuildManager->m_OnChanged.AddUObject(this, &UWidgetBuildPanel::UpdateFurnitureTab);

	UpdateFurnitureTab();
}

void UWidgetBuildPanel::ClosePanel()
{
	Super::ClosePanel();

	UMyGameInstance::Get->m_BuildManager->EndBuilding();
	
	UMyLib::GetPlayerCon()->EnableJoystick(true);

	OnCancel();
	
	UMyLib::GetPlayerCon()->m_OnTouch.Remove(m_DeleTouchWorld);

	UMyGameInstance::Get->m_BuildManager->m_OnCancel.Remove(m_DeleCancel);

	UMyGameInstance::Get->m_BuildManager->m_OnChanged.Remove(m_DeleFurniture);

	UMyLib::GetCanvas()->ShowMainHUD(true);
}

void UWidgetBuildPanel::OnClickStruct()
{
	m_ScrollElements->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_ScrollFurnitureElements->SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_BuildManager->Cancel();
	UMyGameInstance::Get->m_BuildManager->CancelSelect();
	UMyGameInstance::Get->m_BuildManager->SetFurnitureHide();
}

void UWidgetBuildPanel::OnClickFurniture()
{
	m_ScrollElements->SetVisibility(ESlateVisibility::Collapsed);
	m_ScrollFurnitureElements->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UMyGameInstance::Get->m_BuildManager->Cancel();
	UMyGameInstance::Get->m_BuildManager->CancelSelect();
	UMyGameInstance::Get->m_BuildManager->SetWallStructureHide();
}

void UWidgetBuildPanel::OnCancel()
{
	if(m_Focused)
	{
		m_Focused->MyUnFocus();
		m_Focused = nullptr;
	}
	
}

void UWidgetBuildPanel::UpdateFurnitureTab()
{
	m_ScrollFurnitureElements->ClearChildren();
	
	for(auto& Furniture : UMyLib::GetBuildManager()->GetInvenFurniture())
	{
		UWidgetBuildElement* SelectButton = CreateWidget<UWidgetBuildElement>(this,m_ClassBuildEle);
		
		const FBuildDataRow* Data = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(Furniture.Key, "");
		
		SelectButton->Init(*Data);

		SelectButton->SetStackCount(Furniture.Value);

		SelectButton->m_OnClick.BindUObject(this, &UWidgetBuildPanel::OnClickElement);
		
		m_ScrollFurnitureElements->AddChild(SelectButton);
	}
}
