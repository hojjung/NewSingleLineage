#include "WidgetItemObtainPanel.h"

#include "WidgetItemObtainElement.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Managers/RewardManager.h"

void UWidgetItemObtainPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_nIter = 0;
	
	UMyGameInstance::Get->m_Inven->m_OnItemObtain.AddUObject(this, &UWidgetItemObtainPanel::OnItemObtain);

	FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer,this,&UWidgetItemObtainPanel::OnItemObtainQue,1,true,0.15f);
}

void UWidgetItemObtainPanel::OnItemObtain(const FItemDataRow& item, int amount)
{	//겹칠때 어떻게 딜레이 시켜줌?
	m_QueObtained.Enqueue(FItemObtainQue(&item,amount));
}

void UWidgetItemObtainPanel::OnItemObtainQue()
{
	FItemObtainQue Obtain;

	if(!m_QueObtained.Dequeue(Obtain))
	{
		return;
	}
	
	Cast<UWidgetItemObtainElement>(GetTextElement())->ShowImageText(*Obtain.m_Data,Obtain.m_nAmount);

	m_nIter++;
}
