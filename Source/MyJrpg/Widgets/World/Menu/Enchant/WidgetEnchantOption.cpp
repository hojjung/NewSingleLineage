#include "WidgetEnchantOption.h"

void UWidgetEnchantOption::SetBeforeAfter(const FString&& infoText, const FString&& formatText, int beforeValue, int afterValue)
{
	m_TextInfo->SetText(FText::FromString(infoText));

	FStringFormatOrderedArguments ArgsBefore;
	
	ArgsBefore.Add(beforeValue);

	FStringFormatOrderedArguments ArgsAfter;
	
	ArgsAfter.Add(afterValue);

	FString Before = FString::Format(*formatText,ArgsBefore);

	FString After = FString::Format(*formatText,ArgsAfter);

	m_TextLevelBefore->SetText(FText::FromString(Before));
	
	m_TextLevelAfter->SetText(FText::FromString(After));
}
