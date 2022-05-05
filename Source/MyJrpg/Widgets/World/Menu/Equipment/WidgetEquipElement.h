// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetEquipElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetEquipElement : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ExposeOnSpawn="true"))
	UTexture2D* m_TextureEquipType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ElementBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayEquip;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImgEquipIcon;
	
	EEquipSlotType m_Slot;
	
protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;

	FText GetFocusText();

	void OnHoldingComplete();
	
public:
	void SetSlot(EEquipSlotType slot);

	void UpdateElement();

	void UpdateElement(const FName& spec);
	
	void Clear();

	void UnEquipItem();
	
	void SetMyFocus();

	void SetMyUnFocus();

	bool IsSlotEmpty() const;

	FName GetEquippedItem() const;
};
