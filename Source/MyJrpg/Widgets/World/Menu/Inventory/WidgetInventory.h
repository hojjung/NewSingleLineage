// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetFilterBtns.h"

#include "WidgetInventory.generated.h"

class UDDO_InvenItem;
class UInventory;


UCLASS()
class MYJRPG_API UWidgetInventory : public UUserWidget//일부러 패널안쓰는거임
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnFocus, UWidgetBaseElement*, UInventory*, int);

	FOnFocus m_OnFocus;

	FOnFocus m_OnFocusConfirm;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetBaseElement> m_ClassWidgetItemEle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_InvenBox;
	UPROPERTY()
	TArray<UWidgetBaseElement*> m_AryItemEles;
	
	TWeakObjectPtr<UWidgetBaseElement> m_CurrentFocused;
	
	TWeakObjectPtr<UInventory> m_CurrentInven;

	FDelegateHandle  m_InvenDele;

protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	//prevent penetrate touch
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void CreateGridElements();

	void UpdateInventory();

	void OnFocused(UWidgetBaseElement* ele);

	void OnDrag(UWidgetBaseElement* ele);

	void OnDrop(UWidgetBaseElement* ele);

	void SetItem(UWidgetBaseElement* target,const FItemSpec& itemSpec);

public:
	void Init(UInventory* inven);

	void OpenPanel() ;

	void ClosePanel() ;

	void Clear();

	UInventory* GetInven() const;

	void UnFocusCurrent();
};