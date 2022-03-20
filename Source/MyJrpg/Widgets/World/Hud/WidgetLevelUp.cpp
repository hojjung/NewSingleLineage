
#include "WidgetLevelUp.h"

void UWidgetLevelUp::SetLevel(int level)
{
	FString Str = FString::Printf(TEXT("%d"),level);
	
	m_TextLevel->SetText(FText::FromString(Str));
}
