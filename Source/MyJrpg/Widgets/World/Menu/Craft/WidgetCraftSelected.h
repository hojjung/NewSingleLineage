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
	UPROPERTY(EditDefaultsOnly)
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
	UPROPERTY()
	TArray<UWidgetCraftCostElement*> m_AryEle;

	FDelegateHandle m_Dele;

	
protected:
	virtual void NativeOnInitialized() override;

	void UpdateCraftCostPanel();
	
public:
	void SelectCraft(const FCraftable& data);

	void Close();
};
