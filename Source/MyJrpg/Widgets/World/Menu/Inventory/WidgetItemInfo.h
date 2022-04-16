// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetItemElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
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
	UWidgetItemElement* m_ItemIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemName;
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
	UScrollBox* m_ScrollInfo;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetCollecStatChild> m_ClassOption;
	UPROPERTY()
	TArray<UWidgetCollecStatChild*> m_AryOptions;
protected:
	TWeakObjectPtr<UInventory> m_Inven;
	UPROPERTY()
	int m_nEraseAmount;
	UPROPERTY()
	FName m_ItemKey;
	UPROPERTY()
	FName m_CollecID;
	UPROPERTY()
	int m_nCollecIndex;

	
	
protected:
	virtual void NativeOnInitialized() override;;
	
	void SetTypeInfo(EItemInfo info, EItemType type,const FItemDataRow& ItemData);

	void UpdateStat(const FName& target, int level);

	void CreateOption(const FString&& infoText, const FString&& formatText, int v);

	void CreateOption(TSubclassOf<UOptionBase> op, int lv);

	int GetMax();

	void EraseConfirm(int am);

	void EraseConfirm();

	void UpdateRegisterBtn();

	void UpdateEnchantBtn();

	FText GetTypeText(EItemType t);
	
public:
	void SetItemInfo(EItemInfo info,const FName& oID, UInventory* inven);

	void SetCollecItemInfo(const FName& collecID,int index,const FName& oID);
	
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnErase();
	UFUNCTION()
	void OnEnchant();
	UFUNCTION()
	void OnRegister();
};

//획득처가있어야함

