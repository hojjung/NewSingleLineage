#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "WidgetPickpocketPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPickpocketPanel : public UWidgetBasePanel
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TxtStorageInvenCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_OtherPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetFilterBtns* m_FilterBtns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetInventory* m_InvenPanel;
	UPROPERTY()
	int m_nCrntIndex;

	FDelegateHandle m_EachInvenHandle;

	UPROPERTY()
	AMonsterPawn* m_TargetPawn;
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void UpdateText();
	
	void AddInvenDelegate();
	
	void RemoveInvenDelegate();
public:
	void SetTargetPawn(AMonsterPawn* targetPawn);
	
	virtual void OpenPanel() override;

	virtual void ClosePanel() override;

	AMonsterPawn* GetCurrentTargetPawn();
};
