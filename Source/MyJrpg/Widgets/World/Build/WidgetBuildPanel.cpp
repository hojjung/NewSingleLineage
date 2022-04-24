#include "WidgetBuildPanel.h"
#include "MyJrpg/Managers/ConstructionManager.h"
#include "MyJrpg/Actors/Field/StructureActor.h"
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

	FVector Loc = UMyLib::GetPlayer()->GetActorLocation();
	
	UMyGameInstance::Get->m_BuildManager->SpawnPreviewActor(Loc, &data);
}

void UWidgetBuildPanel::OnTouchWorld(const FHitResult& hit)
{
	AStructureActor* SActor = Cast<AStructureActor>(hit.Actor.Get());
	
	if(SActor)
	{
		//TODO: 이미 지어진 액터를 선택하고, 삭제와 업그레이드 옵션이 떠야함
		//그냥 하면 안됨.선택된 데이터가 벽일때는 판을 터치할수있음
		//벽은 그리드 사이에 단한개씩 들어가야 정상아닌가?
		//return;		
	}
	else if(UMyGameInstance::Get->m_BuildManager->GetPreview())
	{
		//UMyGameInstance::Get->m_BuildManager->Cancel();
		//return;
	}
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
}

void UWidgetBuildPanel::OnCancel()
{
	if(m_Focused)
	{
		m_Focused->MyUnFocus();
		m_Focused = nullptr;
	}
	
}