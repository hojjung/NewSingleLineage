#include "WidgetBuildPanel.h"

#include "MyJrpg/MyLib.h"
#include "MyJrpg/Actors/Field/Build/StructureActor.h"
#include "MyJrpg/Managers/ConstructionManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetBuildPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	m_Pl = UMyLib::GetPlayer();

	m_AryEles.Reset();
	
	const TArray<FBuildDataRow*>& AryDatas = UMyGameInstance::Get->m_BuildManager->GetAryBuildDatas();

	for(const FBuildDataRow* Data : AryDatas)
	{
		if(Data->m_BuildType == EBuildType::Furniture || Data->m_BuildType == EBuildType::Field || Data->m_AryCostItem.Num() < 1)
		{
			continue;
		}
		UWidgetBuildElement* SelectButton = CreateWidget<UWidgetBuildElement>(this,m_ClassBuildEle);
		SelectButton->Init(*Data);
		SelectButton->CreateCostWidgets(Data->m_AryCostItem);
		SelectButton->m_OnClick.BindUObject(this, &UWidgetBuildPanel::OnClickElement);
		m_ScrollElements->AddChild(SelectButton);
		m_AryEles.Add(SelectButton);
	}
	
	m_ScrollFurnitureElements->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnStruct->OnClicked.AddDynamic(this, &UWidgetBuildPanel::OnClickStruct);
	m_BtnFurniture->OnClicked.AddDynamic(this, &UWidgetBuildPanel::OnClickFurniture);
}

void UWidgetBuildPanel::OnClickElement(UWidgetBuildElement* ele, const FBuildDataRow& data)
{
	if(m_Focused.Get())
		m_Focused->MyUnFocus();
	m_Focused = ele;
	m_Focused->MyFocus();

	FVector Loc = UMyLib::GetPlayer()->GetActorLocation();
	UMyGameInstance::Get->m_BuildManager->CancelSelect();
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(Loc, &data);
}

void UWidgetBuildPanel::OnTouchWorld(const FHitResult& hit)
{
	AStructureActor* SActor = Cast<AStructureActor>(hit.Actor.Get());
	
	if(SActor && !m_Focused.Get())
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

	m_DeleFlick = UMyLib::GetPlayerCon()->m_OnFlick.AddUObject(this, &UWidgetBuildPanel::OnFlick);

	m_DeleCancel = UMyGameInstance::Get->m_BuildManager->m_OnCancel.AddUObject(this, &UWidgetBuildPanel::OnCancel);
	
	m_DeleFurniture = UMyGameInstance::Get->m_BuildManager->m_OnChanged.AddUObject(this, &UWidgetBuildPanel::UpdateFurnitureTab);

	UpdateFurnitureTab();
	//
	m_DeleInven = UMyGameInstance::Get->m_Inven->m_OnInvenChanged.AddUObject(this, &UWidgetBuildPanel::UpdateElement);

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		m_DeleBag = UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().AddUObject(this, &UWidgetBuildPanel::UpdateElement);
	}
	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		m_DeleBelt = UMyGameInstance::Get->m_EquipManager->GetOnBeltChanged().AddUObject(this, &UWidgetBuildPanel::UpdateElement);
	}

	UpdateElement();
}

void UWidgetBuildPanel::ClosePanel()
{
	Super::ClosePanel();
	
	m_Pl->ClearCameraOffset();

	OnClickStruct();

	UMyGameInstance::Get->m_BuildManager->EndBuilding();
	
	UMyLib::GetPlayerCon()->EnableJoystick(true);

	OnCancel();
	
	UMyLib::GetPlayerCon()->m_OnTouch.Remove(m_DeleTouchWorld);

	UMyLib::GetPlayerCon()->m_OnFlick.Remove(m_DeleFlick);

	UMyGameInstance::Get->m_BuildManager->m_OnCancel.Remove(m_DeleCancel);

	UMyGameInstance::Get->m_BuildManager->m_OnChanged.Remove(m_DeleFurniture);

	UMyLib::GetCanvas()->ShowMainHUD(true);
	//
	UMyGameInstance::Get->m_Inven->m_OnInvenChanged.Remove(m_DeleInven);

	if(UMyGameInstance::Get->m_EquipManager->GetBag())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBagChanged().Remove(m_DeleBag);
	}
	if(UMyGameInstance::Get->m_EquipManager->GetBelt())
	{
		UMyGameInstance::Get->m_EquipManager->GetOnBeltChanged().Remove(m_DeleBelt);
	}
}

void UWidgetBuildPanel::OnFlick(const FVector2D& delta)
{
	FVector2D CamOff = m_Pl->GetCameraOffset();

	CamOff += delta;
	
	m_Pl->SetCameraOffset(CamOff);
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
	if(m_Focused.Get())
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
		if(Furniture.Value < 1)
		{
			continue;
		}
		
		UWidgetBuildElement* SelectButton = CreateWidget<UWidgetBuildElement>(this,m_ClassBuildEle);
		
		const FBuildDataRow* Data = UBuildData::GetBuildTable->FindRow<FBuildDataRow>(Furniture.Key, "");
		
		SelectButton->Init(*Data);

		SelectButton->SetStackCount(Furniture.Value);

		SelectButton->m_OnClick.BindUObject(this, &UWidgetBuildPanel::OnClickElement);
		
		m_ScrollFurnitureElements->AddChild(SelectButton);
	}
}

void UWidgetBuildPanel::UpdateElement()
{
	for(UWidgetBuildElement* Ele : m_AryEles)
	{
		Ele->UpdateCost();
	}
}
