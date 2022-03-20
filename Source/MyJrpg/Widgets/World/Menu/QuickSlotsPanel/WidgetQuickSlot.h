// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "MyJrpg/Items/ItemExecuteBase.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetItemElement.h"

#include "WidgetQuickSlot.generated.h"

class UWidgetCooldownProgress;
struct FItemSpec;
class UWidgetQuickslotBar;
/**
 * 스킬 혹은 아이템을 등록을 시켜준다
 * 등록된 다음 어떻게?
 * 
 */
UCLASS()
class MYJRPG_API UWidgetQuickSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ItemElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCooldownProgress* m_CdProgress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemAmount;
	UPROPERTY()
	int m_nIndex;

	TWeakObjectPtr<UWidgetQuickslotBar> m_ParentBar;
	
protected:
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void UseQuick();

	void SetQuickSlotClear();

	void UpdateSkillSlot();

	const FSkillDataRow* GetSkillData();
	
public:
	void UpdateQuickSlot();//동적할당?,스킬도 되고,아이템도 된다.

	bool IsSlotEmpty();
	void OnSkillUseCountCD();

	void Init(int index,UWidgetQuickslotBar* slotParent);
};

