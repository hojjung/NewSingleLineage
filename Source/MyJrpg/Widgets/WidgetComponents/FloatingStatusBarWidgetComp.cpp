#include "FloatingStatusBarWidgetComp.h"

#include "MyJrpg/Widgets/World/CommonElements/MaterialProgressBar.h"

UFloatingStatusBarWidgetComp::UFloatingStatusBarWidgetComp()
{
	SetManuallyRedraw(true);
}

void UFloatingStatusBarWidgetComp::BeginPlay()
{
	Super::BeginPlay();

}

void UFloatingStatusBarWidgetComp::SetHealthPercentage(float HealthPercentage)
{
	UUserWidget* WidgetObj = GetUserWidgetObject();

	if(!WidgetObj)
	{
		return;
	}
	
	UMaterialProgressBar* WidgetProgressBar = Cast<UMaterialProgressBar>(WidgetObj);
	
	WidgetProgressBar->SetProgressValue(HealthPercentage);

	RequestRedraw();
}//Hide show할때마다 다시 위젯 만드는ㄷㅅ
