// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetCollecItemEle.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCollecItemEle : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocus,UWidgetCollecItemEle*);

	FOnFocus m_OnFocus;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_IconEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_Checkbox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextEnchantLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgLock;
	
	FName m_CollecID;

	FName m_ItemID;

	int m_nIndex;

	int m_nEnchantLv;

	bool m_bRegisterable;

	bool m_bIsRegistered;

	bool m_bIsEquip;

protected:
	void UpdateEquipItem();

	void UpdateMiscItem();

	void OnClicked(UWidgetBaseElement* ele);

	void OnHoldComplete(UWidgetBaseElement* ele);
	
public:
	void Init(const FName& collecID, int index, bool is_equip, const FItemDataHandle& item, int lv);
	
	void Update();

	bool GetIsRegistered();

	const FName& GetItemID() const;

	const FName& GetCollecID() const;

	int GetItemIndex() const;
	
	void SetMyUnfocus();
	
	void SetMyFocus();
};
