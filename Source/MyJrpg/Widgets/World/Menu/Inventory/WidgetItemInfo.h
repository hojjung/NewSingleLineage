// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Items/Inventory.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/Menu/Collection/WidgetCollecItemEle.h"
#include "MyJrpg/Widgets/World/Menu/Collection/WidgetCollecStatChild.h"
#include "MyJrpg/Widgets/World/Menu/StackSelector/WidgetStackCalculator.h"
#include "WidgetItemInfo.generated.h"

/**
 * Open Mode 여러개
 * 장비 아이템
 * 일반 아이템
 * 몬스터
 * 스킬북
 * 
 */
UCLASS()
class MYJRPG_API UWidgetItemInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* m_VertItemOptions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemEffectTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEraseItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnchant;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRegister;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSplit;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetCollecStatChild> m_ClassOption;
	UPROPERTY()
	TArray<UWidgetCollecStatChild*> m_AryOptions;
protected:
	TWeakObjectPtr<UInventory> m_Inven;
	UPROPERTY()
	int m_nEraseAmount;

	FItemSpec* m_ItemSpec;
	
	FName m_CollecID;

	FName m_CollecTargetItemID;

	int m_nCollecIndex;

protected:
	virtual void NativeOnInitialized() override;;
	
	void UpdateStat(const FItemDataRow& target, int level);

	void CreateOption(const FString&& infoText, const FString&& formatText, int v);

	void CreateOption(TSubclassOf<UOptionBase> op, int lv);

	void SetTypeText(EItemType t);
	
protected:
	void UpdateRegisterBtn();

	void UpdateEnchantBtn();

	void UpdateSplitBtn();
	
	void UpdateEraseBtn();

protected:
	int GetMax();

	void SplitConfirm(int am);

	void EraseConfirm();
	
public:
	void SetItemInfo(FItemSpec& item, UInventory* inven);

	void SetInfoItemData(const FItemDataRow& data_row);

	void SetCollecItemInfo(const FName& collecId, int collecIndex, const FName& itemID);
public:
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnErase();
	UFUNCTION()
	void OnEnchant();
	UFUNCTION()
	void OnRegister();
	UFUNCTION()
	void OnSplit();
};

//획득처가있어야함

