// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetItemElement.generated.h"

class AMonsterPawn;
class UWidgetGaugeBar;
class UWidgetInventory;
class UDDO_InvenItem;
struct FItemDataRow;
/**
 * 인벤
 * 아이템 인포
 * 퀵슬롯에서 사용
 */
UCLASS()
class MYJRPG_API UWidgetItemElement : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnClicked,UWidgetItemElement*);

	FOnClicked m_OnFocus;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ElementBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextStackAmount;
	UPROPERTY()
	int m_nIndex;
	
	TWeakObjectPtr<UInventory> m_Inven;
	
	EPanelType m_PanelType;

	int m_nAmount;

	int m_nMaxAmount;

protected:
	FText GetFocusText();

	void OnHoldingComplete();

	void MoveItem(UInventory* addHere,UInventory* removeHere,const FName& ItemSpec, bool IsEquipItem);

	void TryPickPocketItem(UInventory* addHere,AMonsterPawn* target,const FName& ItemSpec, bool IsEquipItem);

	void SellItem();

	void OnSellConfirm(int amount);

	int GetMaxAmount();
	
	void UpdateElement();

	FText GetPickPocketText();
	
public:
	void Init(EPanelType panel,UInventory* inven);

	void SetIndex(int index);
	
	FName GetItemID() const;

	void UpdateElement(const FName& id);
	
	void Clear();

	void UseItem();
	
	void OnClicked();

	void SetMyUnFocus();

	bool IsSlotEmpty();

	int GetIndex();

	void SetMyInteractable(bool isInteractAble);
};
