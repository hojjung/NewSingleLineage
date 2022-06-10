#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "MyJrpg/Widgets/World/Menu/Storage/WidgetStorage.h"
#include "WidgetPickpocketPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPickpocketPanel : public UWidgetStorage
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtStorageInvenCount;
	
	TWeakObjectPtr<AMonsterPawn> m_TargetPawn;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void UpdateText();

	virtual void OnPlInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index) override;

	virtual void OnPlInvenFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index) override;

	virtual void OnStorageInvenFocused(UWidgetBaseElement* ele, UInventory* inven, int index) override;
	
	virtual void OnStorageFocuseConfirm(UWidgetBaseElement* ele, UInventory* inven, int index) override;

	float GetSuccessPercent(const FItemDataRow& itemData);
	
public:
	void SetTargetPawn(AMonsterPawn* targetPawn);
	
	AMonsterPawn* GetCurrentTargetPawn();
};
