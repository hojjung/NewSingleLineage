#pragma once

#include "CoreMinimal.h"
#include "ImageText.h"
#include "WidgetCraftFilterBtns.h"
#include "WidgetCraftSelected.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/Managers/CraftManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "WidgetCraftPanel.generated.h"

class UWidgetFilterBtns;
struct FItemDataRow;
class UWidgetCraftCostElement;
class UWidgetCraftableElement;
/**
 * 아이템만 있는게 아니라 가구도 존재함.
 * 소팅은 매번 돌면 비효울적이니 매니저에서 배열을 끝내놔야한다
 * 가구랑 아이템을 어떻게 동시에 출력?
 */
UCLASS()
class MYJRPG_API UWidgetCraftPanel : public UWidgetBasePanel
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetCraftableElement> m_ClassCraftableElement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftSelected* m_Selected;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_GridEles;
	
	FDelegateHandle m_DeleUpdate;

protected:
	virtual void NativeOnInitialized() override;
	
	void CreateAllCraftWidget(const TArray<FCraftable>& ary);

	void OnSelectCraftItem(const FCraftable& data, int index);
public:
	virtual void ClosePanel() override;

	virtual void OpenPanel() override;
};
