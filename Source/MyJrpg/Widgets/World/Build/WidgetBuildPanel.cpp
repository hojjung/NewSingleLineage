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
		UWidgetBuildElement* SelectButton = CreateWidget<UWidgetBuildElement>(this,m_ClassBuildEle);

		SelectButton->Init(*Data);
		SelectButton->m_OnClick.BindUObject(this, &UWidgetBuildPanel::OnClickElement);

		if(Data->m_BuildType == EBuildType::Furniture)
		{
			m_ScrollFurnitureElements->AddChild(SelectButton);
		}
		else
		{
			m_ScrollElements->AddChild(SelectButton);
		}
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
	
	m_Dele = UMyLib::GetPlayerCon()->m_OnTouch.AddUObject(this, &UWidgetBuildPanel::OnTouchWorld);

	UMyGameInstance::Get->m_BuildManager->StartBuilding();

	UMyGameInstance::Get->m_BuildManager->m_OnCancel.AddUObject(this, &UWidgetBuildPanel::OnCancel);
}

void UWidgetBuildPanel::ClosePanel()
{
	Super::ClosePanel();

	UMyGameInstance::Get->m_BuildManager->EndBuilding();
	
	UMyLib::GetPlayerCon()->EnableJoystick(true);

	OnCancel();
	
	UMyLib::GetPlayerCon()->m_OnTouch.Remove(m_Dele);

	UMyGameInstance::Get->m_BuildManager->m_OnCancel.Remove(m_Dele2);

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
