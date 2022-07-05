#include "WidgetMapPanel.h"

#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/World/Menu/Craft/WidgetCraftPanel.h"

void UWidgetMapPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_ZoneMove->ClearWidgetMap();

	TArray<UWidget*> AryWidgets = m_CanvasMap->GetAllChildren();

	for(UWidget* ChildWidget :  AryWidgets)
	{
		UWidgetMapBtn* Btn = Cast<UWidgetMapBtn>(ChildWidget);

		if(!Btn)
			continue;

		Btn->m_OnClick.BindUObject(this, &UWidgetMapPanel::OnClick);
		
		UMyGameInstance::Get->m_ZoneMove->AddMapBtn(Btn->GetZoneID(),Btn);
	}

	m_ZoneSelect->SetVisibility(ESlateVisibility::Collapsed);

	m_PanelSlot = Cast<UCanvasPanelSlot>(m_CanvasMap->Slot);

	m_ItemInfo->SetVisibility(ESlateVisibility::Collapsed);

	m_InvenEquip->SetVisibility(ESlateVisibility::Collapsed);

	m_CraftPanel->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnCraft->OnClicked.AddDynamic(this, &UWidgetMapPanel::OnOpenCraft);

	m_BtnInven->OnClicked.AddDynamic(this, &UWidgetMapPanel::OnOpenInven);


	m_MoveBar->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnRun->OnClicked.AddDynamic(this, &UWidgetMapPanel::OnRunStart);

	UMyGameInstance::Get->m_ZoneMove->m_OnMoveStart.AddUObject(this, &UWidgetMapPanel::SetMoveBar);
	UMyGameInstance::Get->m_ZoneMove->m_OnMoveStart.AddUObject(this, &UWidgetMapPanel::OnWalkStart);
	
	UMyGameInstance::Get->m_ZoneMove->m_OnMoveTick.AddUObject(this, &UWidgetMapPanel::OnMove);
	UMyGameInstance::Get->m_ZoneMove->m_OnMoveTick.AddUObject(this, &UWidgetMapPanel::OnWalkTick);

	UMyGameInstance::Get->m_ZoneMove->m_OnMoveEnd.AddUObject(this, &UWidgetMapPanel::OnMoveEnd);

	OnMove();
	
	m_BtnRun->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetMapPanel::OpenItemInfoData(const FItemDataRow& item_data_row)
{
	m_ItemInfo->SetInfoItemData(item_data_row);
}

void UWidgetMapPanel::PrintErrorText(const FString& string)
{
	m_AlertInfoWindow->PrintErrorText(string);
}

void UWidgetMapPanel::OnClick(const FName& zoneID)
{
	m_ZoneSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	const FZoneDataRow* ZoneData = UZoneData::GetZoneTable->FindRow<FZoneDataRow>(zoneID,"");

	m_ZoneSelect->Init(*ZoneData);
}

FReply UWidgetMapPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetMapPanel::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	
	return FReply::Handled();
}

void UWidgetMapPanel::OnWalkStart(const FName& dst, float dist)
{
	if(UMyGameInstance::Get->m_ZoneMove->IsRunning())
	{
		return;
	}
	m_BtnRun->SetVisibility(ESlateVisibility::Visible);
	
	FVector2D DestPos = UMyGameInstance::Get->m_ZoneMove->GetDestPos();

	Cast<UCanvasPanelSlot>(m_BtnRun->Slot)->SetPosition(DestPos);
}

void UWidgetMapPanel::OnWalkTick()
{
	if(UMyGameInstance::Get->m_ZoneMove->IsRunning())
	{
		return;
	}

	FVector2D PlDest = Cast<UCanvasPanelSlot>(m_PlayerIcon->Slot)->GetPosition();

	float Dist = UMyGameInstance::Get->m_ZoneMove->GetDist(PlDest);
	
	int Cost = 0;
	
	float Time = 0;
	
	UMyGameInstance::Get->m_ZoneMove->GetRunStaminaCostTime(Dist, Cost, Time);

	m_TextRunCost->SetText(FText::AsNumber(Cost));

	const FString& CultName = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	
	FCulturePtr Culture = FInternationalization::Get().GetCulture(CultName);
	
	FTimespan Run(0,0,Time);
	
	m_TextRunTimeSpan->SetText(FText::AsTimespan(Run, Culture));
}

void UWidgetMapPanel::OnOpenInven()
{
	m_InvenEquip->OpenPanel();
}

void UWidgetMapPanel::OnOpenCraft()
{
	m_CraftPanel->OpenPanel();
}

void UWidgetMapPanel::OnRunStart()
{
	m_BtnRun->SetVisibility(ESlateVisibility::Collapsed);
	
	FVector2D PlDest = Cast<UCanvasPanelSlot>(m_PlayerIcon->Slot)->GetPosition();
	
	int Cost = 0;
	
	float Time = 0;

	FName Dst = UMyGameInstance::Get->m_ZoneMove->GetDestZoneID();

	float Dist = UMyGameInstance::Get->m_ZoneMove->GetDist(PlDest);
	
	UMyGameInstance::Get->m_ZoneMove->GetRunStaminaCostTime(Dist, Cost, Time);
	
	UMyGameInstance::Get->m_ZoneMove->StartMove(true, Time, Dst);
}

void UWidgetMapPanel::SetMoveBar(const FName& dst, float dist)
{
	m_MoveBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_PlayerIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	FVector2D BarPos =  UMyGameInstance::Get->m_ZoneMove->GetBarPos(dst);

	Cast<UCanvasPanelSlot>(m_MoveBar->Slot)->SetPosition(BarPos);

	Cast<UCanvasPanelSlot>(m_MoveBar->Slot)->SetSize(FVector2D(dist,35));

	float EuletAngle = UMyGameInstance::Get->m_ZoneMove->GetEulerAngle(dst);;

	m_MoveBar->SetRenderTransformAngle(EuletAngle);
}

void UWidgetMapPanel::OnMove()
{
	FVector2D IconPos = UMyGameInstance::Get->m_ZoneMove->GetPlayerIconPos();
	
	Cast<UCanvasPanelSlot>(m_PlayerIcon->Slot)->SetPosition(IconPos);
}

void UWidgetMapPanel::OnMoveEnd()
{
	m_MoveBar->SetVisibility(ESlateVisibility::Collapsed);

	m_BtnRun->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UWidgetMapPanel::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	FVector2D OutCanvasSize = InGeometry.GetLocalSize() / 2.f;

	FVector2D PanelSize = m_PanelSlot->GetSize() / 4.f;
	
	float RateX =  PanelSize.X / OutCanvasSize.X;

	float RateY =  PanelSize.Y / OutCanvasSize.Y;

	PanelSize.X *= RateX;

	PanelSize.Y *= RateY;

	FVector2D Pos = m_PanelSlot->GetPosition();

	const FVector2D& Delta = InGestureEvent.GetCursorDelta();

	FVector2D ResultPos = Pos + (Delta * 2.f);

	ResultPos.X = FMath::Clamp<float>(ResultPos.X, -PanelSize.X, PanelSize.X);

	ResultPos.Y = FMath::Clamp<float>(ResultPos.Y, -PanelSize.Y, PanelSize.Y);

	m_PanelSlot->SetPosition(ResultPos);

	return FReply::Handled();
}


