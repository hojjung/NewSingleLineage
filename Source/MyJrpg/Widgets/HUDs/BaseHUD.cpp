#include "BaseHUD.h"

void ABaseHUD::PrintInfoText(FText&& tt, FLinearColor color)
{
	PrintInfoText(tt.ToString(),color);
}

void ABaseHUD::OpenItemInfo(const FItemDataRow& item_data)
{
	
}

void ABaseHUD::PrintInfoText(const FString& str, FLinearColor color)
{
	
}
