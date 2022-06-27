#include "WidgetMinimapPanel.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetMinimapPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_ZoneInst->m_OnActorVisible.BindUObject(this, &UWidgetMinimapPanel::OnActorVisible);

	m_PlCon = UMyLib::GetPlayerCon();
	
	int SizeX, SizeY;
		
	m_PlCon->GetViewportSize(SizeX,SizeY);

	m_ScreenSize.X = SizeX;

	m_ScreenSize.Y = SizeY;

	OnActorVisible(UMyLib::GetPlayer(),true);
}

void UWidgetMinimapPanel::OnActorVisible(AActor* actorTarget, bool isVisible)
{
	AActor** FoundActorPtr = m_VisibleActors.Find(actorTarget);

	if(!FoundActorPtr && isVisible)
	{
		m_VisibleActors.Add(actorTarget);

		UWidgetMinimapIcon* MapIcon = CreateWidget<UWidgetMinimapIcon>(this, m_ClassWidgetMinimapIcon);

		UCanvasPanelSlot* MapIconSlot = m_MainMapCanvas->AddChildToCanvas(MapIcon);

		MapIconSlot->SetAutoSize(true);
		
		m_MapIcons.Add(actorTarget, MapIconSlot);

	}
	else if(FoundActorPtr && !isVisible)
	{
		m_VisibleActors.Remove(actorTarget);
		
		UCanvasPanelSlot* FoundSlot = m_MapIcons.FindAndRemoveChecked(actorTarget);

		FoundSlot->Content->RemoveFromParent();
	}
}

void UWidgetMinimapPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	m_CanvasSize = MyGeometry.GetLocalSize();

	m_RatioSize = m_CanvasSize / m_ScreenSize; 

	for(auto TupleActorWidgetPair : m_MapIcons)
	{
		FVector Loc = TupleActorWidgetPair.Key->GetActorLocation();

		FVector2D ScreenLoc;//화면상의 위치

		UGameplayStatics::ProjectWorldToScreen(m_PlCon.Get(), Loc, ScreenLoc);

		FVector2D NewScaleSize = m_RatioSize * ScreenLoc;

		TupleActorWidgetPair.Value->SetPosition(NewScaleSize);
	}
}