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
	UButton* m_BtnRide;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRunCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRunTimeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRideCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextRideTimeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextWalkTimeSpan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* m_ParentBtns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_ParentBoat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_ParentHorse;
	
	const FZoneDataRow* m_ZoneData;

	TSet<FName> m_SetRewardItems;
	
	float m_fDist;
	
	float m_fWalkTime;
	
	int m_RunCost;

	float m_fRunTime;

	int m_RideCost;

	float m_fRideTime;


protected:
	void ShowRideBtn(bool b);

	void ShowRunBtn(bool b);

	void ShowWalkBtn(bool b);
	
	void SetItemsInSet();

	void CreateZoneElement();

	virtual void NativeOnInitialized() override;

	void SetBarGauge(UProgressBar* bar, int amount);
	
	void SetPlayerHome();

	void UpdateBtnText();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void TryShowBoatBtn();
	
	void TryShowHorseBtn();
	
	void SetZone();

	void SetRideText(const FCulturePtr& cPtr);

	void SetRunText(const FCulturePtr& cPtr);

	void SetWalkText(const FCulturePtr& cPtr);
	
public:
	void Init(const FZoneDataRow& zone_data);
	
	UFUNCTION()
	void OnClose();
	UFUNCTION()
	void MoveToZone();
	UFUNCTION()
	void OnWalk();
	UFUNCTION()
	void OnRun();
	UFUNCTION()
	void OnRide();
};
 