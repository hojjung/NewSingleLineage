#include "WidgetItemObtainElement.h"

#include "MyJrpg/MyLib.h"

void UWidgetItemObtainElement::Show(const FString& str)
{
	Super::Show(str);
}

void UWidgetItemObtainElement::ShowImageText(const FItemDataRow& item, int amount)
{
	m_ImgIcon->SetBrushFromSoftTexture(item.m_Icon);

	FString Str;
	
	if(UMyLib::IsEquip(item))
	{
		Str = FString::Printf(TEXT("%s +Lv.%d"),*item.m_ShowingName.ToString(),amount + 1);
	}
	else
	{
		Str = FString::Printf(TEXT("%s x %d"),*item.m_ShowingName.ToString(),amount);
	}
	
    Show(Str);
}