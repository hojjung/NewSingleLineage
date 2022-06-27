#pragma once

#include "CoreMinimal.h"
#include "WidgetCraftCostElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "MyJrpg/Managers/CraftManager.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetCraftSelected.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCraftSelected : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UWidgetCraftCostElement> m_ClassCost; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_Wrap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCraft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextLevelLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextCountLimit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_CancelHoldingBar;
	UPROPERTY()
	TArray<UWidgetCraftCostElement*> m_AryEle;

	FDelegateHandle m_Dele;

	FDelegateHandle m_Dele2;

	FDelegateHandle m_Dele3;

	float m_fCancelTimer;

	bool m_bStartCraft;
	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateCraftCostPanel();

	void SetLimitLevel(const FCraftDataInfo& data);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void SelectCraft(const FCraftDataInfo& data);

	void Close();

	UFUNCTION()
	void Craft();

	UFUNCTION()
	void Cancel();

	bool IsCrafting();
};
