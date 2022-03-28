
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Hud/WidgetGameOver.h"
#include "Hud/WidgetLevelUp.h"
#include "Menu/CharacterInfoHUD/AlertInfoWindow.h"
#include "Menu/Collection/WidgetCollecPanelParent.h"
#include "Menu/Dialogue/WidgetLogWindow.h"
#include "Menu/Enchant/WidgetEnchantBase.h"
#include "Menu/Quest/WidgetQuestAcceptPanel.h"
#include "Menu/Quest/WidgetQuestPanel.h"
#include "Menu/StackSelector/WidgetStackCalculator.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "WidgetCanvasWorld.generated.h"

class UWidgetSkinPanel;
struct FZoneDataRow;
struct FZone;
class UWidgetStackCalculator;
class UWidgetShopPanel;
class UWidgetZoneSelectPanel;
class UWidgetStorage;
class UInventory;
class UWidgetCraftPanel;
class UWidgetSkillPanel;
class UWidgetEquipInvenPanel;
class UButton;
class UWidgetItemInfo;
class UWidgetQuickslotBar;
class UWidgetLevelHealthInfo;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetCanvasWorld : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UAlertInfoWindow* m_AlertInfoWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetLevelHealthInfo* m_LevelHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuickslotBar* m_QuickBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemInfo* m_ItemInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnToggleAuto;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* m_WrapboxMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCashShop;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEquipInven;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnCraft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOption;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnSkin;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextZoneName;
	//
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* ZoneOpen;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* LevelUp;
	UPROPERTY(Transient,BlueprintReadWrite,meta = (BindWidgetAnim,AllowPrivateAccess = "true"))
	UWidgetAnimation* GameOver;
	//LevelName
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEquipInvenPanel* m_EquipInvenPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSkillPanel* m_SkillPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCraftPanel* m_CraftPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetShopPanel* m_ShopPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetStorage* m_StoragePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetZoneSelectPanel* m_ZonePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuestPanel* m_QuestPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetLogWindow* m_DialoguePanel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetStackCalculator* m_Calculator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuestAcceptPanel* m_QuestAcceptPanel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetLevelUp* m_WindowLevelUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetGameOver* m_WindowGameOver;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuestHUD* m_QuestHUD;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetEnchantBase* m_Enchant;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSkinPanel* m_SkinPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetCollecPanelParent* m_ItemCollecPanel;
	
protected:
	void HideHUD();

	void ShowHUD();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnInitialized() override;
	
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;
	
public:
	UWidgetEquipInvenPanel* GetEquipInvenMenu();

	void OpenItemInfo(const FName& itemSpec);

	UFUNCTION()
	void ToggleMenu();
	UFUNCTION()
	void AutoToggle();
	UFUNCTION()
	void OpenSkin();
	UFUNCTION()
	void OpenItemCollec();
	UFUNCTION()
	void OpenQuest();
	UFUNCTION()
	void OpenInventory();
	UFUNCTION()
	void OpenSkill();
	UFUNCTION()
	void OpenCraft();
	UFUNCTION()
	void OpenShop(const UDataTable* dT);
	UFUNCTION()
	void OpenStorage();
	UFUNCTION()
	void OpenZone();

	void OpenQuestPanel(bool isMain);

	UWidgetStackCalculator* OpenCalculator(int cnt);
	
	void StartDialogue(FName str);
	
	void TryAcceptQuest(FName qId);

	void OpenEnchant();
	
public:
	void ShowSkillFail(const FString& string);

	void ShowZone(const FText& nameT);

	void ShowLevelUpWindow();

	void ShowGameOverWindow(const ACombatUnitPawn* killer);
};