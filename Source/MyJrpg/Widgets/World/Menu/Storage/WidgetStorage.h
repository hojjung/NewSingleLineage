// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetStorage.generated.h"

/**
 * 인벤에 해줄게 너무 많다.
 * 인벤이 withdraw와 deposit을 설정하게 해주고
 * 버튼을 분리해주기
 * 그다음 창고를 인벤 두개로 사용해야한다.
 * Update가 문제임
 */
UCLASS()
class MYJRPG_API UWidgetStorage : public UWidgetBasePanel
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtStorageInvenCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_StoragePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetFilterBtns* m_FilterBtns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_InvenPanel;
	UPROPERTY()
	int m_nCrntIndex;

	FDelegateHandle m_EachInvenHandle;
protected:
	virtual void NativeOnInitialized() override;

	virtual void UpdateText();
	
	void AddInvenDelegate();
	
	void RemoveInvenDelegate();
public:
	void OpenPanel();

	virtual void ClosePanel() override;
	UFUNCTION()
	void OnClickLeft();
	UFUNCTION()
	void OnClickRight();
};
