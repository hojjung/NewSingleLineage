// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "WidgetZoneItemElement.h"
#include "WidgetZoneMonsterElement.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "MyJrpg/DataTables/ZoneData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"

#include "WidgetZoneSelectButton.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetZoneSelectButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWidgetZoneItemElement> m_ClassItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnClose;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextMapDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* m_HoriItemParents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarSkull;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarWood;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarRock;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_BarItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnWalk;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRunCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRunTimeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextWalkTimeSpan;
	
	const FZoneDataRow* m_ZoneData;

	TSet<FName> m_SetRewardItems;
	
	int m_RunCost;

	float m_fWalkTime;

	float m_fRunTime;

	float m_fDist;

protected:
	void SetItemsInSet();

	void CreateZoneElement();

	virtual void NativeOnInitialized() override;

	void SetBarGauge(UProgressBar* bar, int amount);
	
	void SetPlayerHome();

	void GetRunStaminaCostTime(int& staminaCost, float& timeSpan);

	void GetWalkTime(float& timeSpan);

	void UpdateBtnText();
	
public:
	void Init(const FZoneDataRow& zone_data);
	
	void SetZone();
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void MoveToZone();
	UFUNCTION()
	void OnWalk();
	UFUNCTION()
	void OnRun();
};
 