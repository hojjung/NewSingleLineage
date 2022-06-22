// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "WidgetZoneItemElement.h"
#include "WidgetZoneMonsterElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"

#include "WidgetZoneSelectButton.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetZoneSelectButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetZoneItemElement> m_ClassItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnterZone;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMapDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_HoriItemParents;

	const FZoneDataRow* m_ZoneData;

	TSet<FName> m_SetRewardItems;
	
	
protected:
	void SetItemsInSet();

	void CreateZoneElement();

	virtual void NativeOnInitialized() override;
	
public:
	void Init(const FZoneDataRow& zone_data);
	
	void SetZone();
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void MoveToZone();
};
 