#include "WidgetBuffPanel.h"
#include "WidgetBuffElement.h"
#include "MyJrpg/Managers/MyGameInstance.h"

void UWidgetBuffPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMyGameInstance::Get->m_PlayerStatManager->m_OnAddBuff.AddUObject(this,&UWidgetBuffPanel::OnBuffAdd);

	UMyGameInstance::Get->m_PlayerStatManager->m_OnRemoveBuff.AddUObject(this,&UWidgetBuffPanel::OnBuffRemove);

	//버프결국 맵순회로 만들어줘야함
}

void UWidgetBuffPanel::OnBuffAdd(UBuff_Base* buff)
{
	if(m_MapBuff.Contains(buff))
	{
		m_MapBuff[buff]->SetBuff(buff);
		
		return;		
	}
	
	UWidgetBuffElement* ItemEle = CreateWidget<UWidgetBuffElement>(this, m_ClassBuffEle);

	ItemEle->SetBuff(buff);

	m_MapBuff.Add(buff,ItemEle);

	m_WrapBox->AddChildToWrapBox(ItemEle);	
}

void UWidgetBuffPanel::OnBuffRemove(UBuff_Base* buff)
{
	m_WrapBox->RemoveChild(m_MapBuff[buff]);

	m_MapBuff.Remove(buff);
}
