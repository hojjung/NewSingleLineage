#include "MinimapManager.h"

#include "MyJrpg/MyLib.h"

void UMinimapManager::Init()
{
	m_AryActors.Reset();

	m_PlCon = UMyLib::GetPlayerCon();
}

void UMinimapManager::AddTrackActor(AActor* target)
{
	m_AryActors.Add(target);
}

bool UMinimapManager::IsVisible(AActor* target)
{
	FVector Loc = target->GetActorLocation();
		
	FVector2D ScreenSize;

	bool Result = m_PlCon->ProjectWorldLocationToScreen(Loc,ScreenSize);

	int SizeX, SizeY;
		
	m_PlCon->GetViewportSize(SizeX, SizeY);

	bool XFit = 0 < ScreenSize.X && ScreenSize.X < SizeX;

	bool YFit = 0 < ScreenSize.Y && ScreenSize.Y < SizeY;

	return Result && XFit && YFit;
}
