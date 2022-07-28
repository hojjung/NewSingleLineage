#pragma once

#include "CoreMinimal.h"
#include "AlertInfoElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "WidgetItemObtainElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetItemObtainElement : public UAlertInfoElement
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgIcon;
	
public:
	void ShowImageText(const FItemDataRow& item, int amount);
};
