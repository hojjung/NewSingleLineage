// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "WidgetZoneItemElement.h"
#include "WidgetZoneMonsterElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
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
	TSubclassOf<UWidgetZoneMonsterElement> m_ClassMonster;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetZoneItemElement> m_ClassItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnterZone;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMapDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* m_HoriMonsterParents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* m_HoriItemParents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeftIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRightIndex;

	const FZoneDataRow* m_ZoneData;

	TSet<const FNpcUnitEntityRow*> m_SetMonsters;
	
	UPROPERTY()
	int m_nIndex;
	
protected:
	void UpdateText();
	
	void CreateMonsters(const FZone& zone_data);

	void CreateItems(const FZone& zone_data);
	
public:
	void Init(const FZoneDataRow& zone_data);
	
	void SetZone(const FZone& zone);

	UFUNCTION()
	void MoveToZone();
	UFUNCTION()
	void OnLeftClick();
	UFUNCTION()
	void OnRightClick();
};
 