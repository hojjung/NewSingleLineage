// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "WidgetItemElement.h"
#include "Blueprint/UserWidget.h"
#include "MyJrpg/Widgets/World/Menu/StackSelector/WidgetStackCalculator.h"
#include "WidgetItemInfo.generated.h"

/**
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
	UTextBlock* m_TextItemDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextItemEffect;//real option
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEraseItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCalculator;
	

	UPROPERTY()
	FName m_ItemKey;
	UPROPERTY()
	int m_nEraseAmount;
	const FItemSpec* m_EquipItem;

protected:
	virtual void NativeOnInitialized() override;;
	
public:
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void OnErase();
	UFUNCTION()
	void OnOpenCalculator();
	

	void SetItemInfo(const FItemSpec& itemSpecInfo);
};


