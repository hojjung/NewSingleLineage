#include "WidgetCollecStatChild.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"


void UWidgetCollecStatChild::SetStatText(TTuple<TSubclassOf<UOptionBase>, TArray<int>> opPair)
{
	UOptionBase* Op = opPair.Key->GetDefaultObject<UOptionBase>();

	if(Op->IsPercent())
	{
		FString StrPercent = FString::Printf(TEXT("%s%%"),*Op->GetOptionName());
		
		m_StatText->SetLeftText(FText::FromString(StrPercent));
	}
	else
	{
		m_StatText->SetLeftText(FText::FromString(Op->GetOptionName()));
	}
	
	FString Str = FString::Printf(TEXT("+%d/%d"), opPair.Value[0], opPair.Value[1]);

	m_StatText->SetRightText(FText::FromString(Str));
}
