
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Build/WidgetBuildPanel.h"
#include "Components/WrapBox.h"
#include "Hud/WidgetGameOver.h"
#include "Hud/WidgetLevelUp.h"
#include "Hud/WidgetMinimapPanel.h"
#include "Hud/WidgetScreenEffect.h"
#include "Hud/WidgetWaitInteract.h"
#include "Menu/CharacterInfoHUD/AlertInfoWindow.h"
#include "Menu/Collection/WidgetCollecPanelParent.h"
#include "Menu/Craft/WidgetAssemble.h"
#include "Menu/Craft/WidgetAssembleAndInven.h"
#include "Menu/Dialogue/WidgetLogWindow.h"
#include "Menu/Enchant/WidgetEnchantBase.h"
#include "Menu/Equipment/WidgetStatHealth.h"
#include "Menu/ItemInteract/WidgetItemConvert.h"
#include "Menu/Pet/WidgetPetPanel.h"
#include "Menu/Pickpocket/WidgetPickpocketPanel.h"
#include "Menu/Quest/WidgetQuestAcceptPanel.h"
#include "Menu/Quest/WidgetQuestPanel.h"
#include "Menu/StackSelector/WidgetStackCalculator.h"
#include "MyJrpg/Actors/Field/Build/BuildInteract/BI_Assemble.h"
#include "MyJrpg/Actors/Field/Build/BuildInteract/BI_ItemConverter.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "WidgetCanvasWorld.generated.h"

class UEquipManager;
class UWidgetConfirmPanel;
struct FZoneDataRow;
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
	UCanvasPanel* m_MainHUD;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UAlertInfoWindow* m_AlertInfoWindow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetStatHealth* m_StatHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetQuickslotBar* m_QuickBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemInfo* m_ItemInfo;
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
	UButton* m_BtnQuest;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnOption;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnBuild;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetMinimapPanel* m_Minimap;
	
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
	UWidgetQuestPanel* m_QuestPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetLogWindow* m_DialoguePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetPickpocketPanel* m_PickpocketPanel;
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
	UWidgetCollecPanelParent* m_ItemCollecPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetPetPanel* m_PetPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBuildPanel* m_BuildPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetItemConvert* m_ItemConvertPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetAssembleAndInven* m_ItemAssemblePanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetConfirmPanel* m_ConfirmPanel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetWaitInteract* m_WaitBar;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetScreenEffect* m_ScreenEffect; 
	//
	bool m_bIsBuildable;
protected:
	void HideHUD();

	void ShowHUD();

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnInitialized() override;
	
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

	void OnZoneMove(const FName& zoneID);
	
public:
	UWidgetEquipInvenPanel* GetEquipInvenMenu();

	UWidgetPickpocketPanel* GetPickpocketMenu();

	UWidgetConfirmPanel* GetConfirmPanel();

	void OpenItemInfoData(const FItemDataRow& item);

	void OpenItemInfo(FItemSpec& item, UInventory* inven);

	void OpenItemInfoCollec(const FName& collecId, int collecIndex, const FName& itemID);
	
public:
	UFUNCTION()
	void ToggleMenu();
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
	void OpenShop(const FName& traderID);
	UFUNCTION()
	void OpenPet();
	UFUNCTION()
	void OpenBuild();

	void OpenStorage(UInventory* storage);
	
	void OpenQuestPanel(bool isMain);

	UWidgetStackCalculator* GetCalculator();
	
	void TryAcceptQuest(FName qId);

	void OpenEnchant();
	
	void PrintErrorText(const FString& string);

	void ShowZone(const FText& nameT);

	void ShowLevelUpWindow();

	void ShowGameOverWindow(const ACombatUnitPawn* killer);
	
	void StartDialogue(FName str, AActor* speaker = nullptr);
	
	void StartPickPocket(AMonsterPawn* mob);

	void ShowMainHUD(bool b);

	UWidgetStorage* GetStorageMenu();
	
	void OpenItemConverter(UItemConvertInst* inst);

	UWidgetWaitInteract* GetWaitInteract();

	UWidgetScreenEffect* GetScreenEffect();
	
	void OpenAssemble(UAssembleInst* assemble_inst);
};

