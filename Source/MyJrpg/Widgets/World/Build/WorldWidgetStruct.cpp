#include "WorldWidgetStruct.h"

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/Managers/ConstructionManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWorldWidgetStruct::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnCancel->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnCancel);

	m_BtnConfirm->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnConfirm);

	m_BtnRotation->OnClicked.AddDynamic(this, &UWorldWidgetStruct::OnRotation);
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
	if(b)
		m_BtnConfirm->SetVisibility(ESlateVisibility::Visible);
	else
		m_BtnConfirm->SetVisibility(ESlateVisibility::Collapsed);
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
