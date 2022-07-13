#include "WidgetCanvasWorld.h"

#include "BUITween.h"
#include "Animation/UMGSequencePlayer.h"
#include "CommonElements/WidgetConfirmPanel.h"
#include "Menu/Craft/WidgetCraftPanel.h"
#include "Menu/Equipment/WidgetEquipInvenPanel.h"
#include "Menu/Inventory/WidgetItemInfo.h"
#include "Menu/QuickSlotsPanel/WidgetQuickslotBar.h"
#include "Menu/Shop/WidgetShopPanel.h"
#include "Menu/Skill/WidgetSkillPanel.h"
#include "Menu/Storage/WidgetStorage.h"
#include "Menu/ZoneMove/WidgetZoneSelectPanel.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/DataTables/DialogueTable.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

void UWidgetCanvasWorld::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_WindowGameOver->SetVisibility(ESlateVisibility::Collapsed);

	m_WindowLevelUp->SetVisibility(ESlateVisibility::Collapsed);
	
	m_TextZoneName->SetVisibility(ESlateVisibility::Collapsed);
	//CloseBtns
	m_WrapboxMenu->SetVisibility(ESlateVisibility::Collapsed);
	//Close Menu
	m_ItemInfo->SetVisibility(ESlateVisibility::Collapsed);

	m_DialoguePanel->SetVisibility(ESlateVisibility::Collapsed);

	m_Enchant->SetVisibility(ESlateVisibility::Collapsed);

	m_QuestAcceptPanel->SetVisibility(ESlateVisibility::Collapsed);
	//Bind Event
	m_BtnMenu->OnClicked.AddDynamic(this,&UWidgetCanvasWorld::ToggleMenu);

	m_BtnEquipInven->OnClicked.AddDynamic(this,&UWidgetCanvasWorld::OpenInventory);

	m_BtnCraft->OnClicked.AddDynamic(this,&UWidgetCanvasWorld::OpenCraft);

	m_BtnQuest->OnClicked.AddDynamic(this,&UWidgetCanvasWorld::OpenQuest);

	m_BtnCashShop->OnClicked.AddDynamic(this,&UWidgetCanvasWorld::OpenCashShop);

	m_Calculator->SetVisibility(ESlateVisibility::Collapsed);

	UMyGameInstance::Get->m_LevelMoveManager->m_OnLvelMoveComp.AddUObject(this, &UWidgetCanvasWorld::OnZoneMove);
	
	UMyGameInstance::Get->m_LevelMoveManager->m_OnLvelMoveCompText.AddUObject(this, &UWidgetCanvasWorld::ShowZone);

	UMyGameInstance::Get->m_PlayerStatManager->m_OnLevelChanged.AddUObject(this, &UWidgetCanvasWorld::ShowLevelUpWindow);

	UMyGameInstance::Get->m_PlayerStatManager->m_OnPlayerKilled.AddUObject(this, &UWidgetCanvasWorld::ShowGameOverWindow);
}

void UWidgetCanvasWorld::ToggleMenu()
{
	m_BtnMenu->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if(m_WrapboxMenu->IsVisible())
	{
		if(m_bIsBuildable)
		{
			m_BtnBuild->SetVisibility(ESlateVisibility::Collapsed);	
		}
		m_Minimap->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_WrapboxMenu->SetVisibility(ESlateVisibility::HitTestInvisible);
		UBUITween::Create(m_WrapboxMenu,0.1f)
		.FromOpacity(1)
		.ToOpacity(0)
		.OnComplete( FBUITweenSignature::CreateLambda([&]( UWidget* Owner )
			{
				Owner->SetVisibility(ESlateVisibility::Collapsed);
				m_BtnMenu->SetVisibility(ESlateVisibility::Visible);
			
			}))
		.Begin();
	}
	else
	{
		if(m_bIsBuildable)
		{
			m_BtnBuild->SetVisibility(ESlateVisibility::Visible);
		}
		m_Minimap->SetVisibility(ESlateVisibility::Collapsed);
		m_WrapboxMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UBUITween::Create(m_WrapboxMenu,0.1f)
		.FromOpacity(0)
		.ToOpacity(1)
		.OnComplete( FBUITweenSignature::CreateLambda([&]( UWidget* Owner )
			{
				m_BtnMenu->SetVisibility(ESlateVisibility::Visible);
			
			}))
		.Begin();
	}

}

void UWidgetCanvasWorld::OpenSkin()
{
	
}

void UWidgetCanvasWorld::OpenItemCollec()
{
	m_ItemCollecPanel->Open();
}

void UWidgetCanvasWorld::OpenQuest()
{
	m_QuestPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCanvasWorld::OpenInventory()
{
	m_EquipInvenPanel->OpenPanel();
}

void UWidgetCanvasWorld::OpenSkill()
{
	m_SkillPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidgetCanvasWorld::OpenCraft()
{
	m_CraftPanel->OpenPanel();
}

void UWidgetCanvasWorld::OpenShop(const FName& traderID)
{
	m_ShopPanel->SetShopPanel(traderID);
}

void UWidgetCanvasWorld::OpenStorage(UInventory* storage)
{
	m_StoragePanel->SetTargetInven(storage);
	m_StoragePanel->OpenPanel();
}

void UWidgetCanvasWorld::OpenPet()
{
	m_PetPanel->Open();
}

void UWidgetCanvasWorld::OpenBuild()
{
	ShowMainHUD(false);
	m_BuildPanel->OpenPanel();
}

void UWidgetCanvasWorld::OpenCashShop()
{
	m_CashShop->OpenPanel();
}

void UWidgetCanvasWorld::OpenQuestPanel(bool isMain)
{
	OpenQuest();
	
	if (isMain)
	{
		m_QuestPanel->ShowMainQuest();
	}
	else
	{
		m_QuestPanel->ShowSubQuest();
	}
}

UWidgetStackCalculator* UWidgetCanvasWorld::GetCalculator()
{
	return m_Calculator;
}

void UWidgetCanvasWorld::StartDialogue(FName str, AActor* speaker)
{
	m_DialoguePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UDialogue* Dial = UDialogueTable::GetDialogue(str);

	m_DialoguePanel->StartDialogue(speaker, Dial);
}

void UWidgetCanvasWorld::TryAcceptQuest(FName qId)
{
	m_QuestAcceptPanel->SetQuestAccept(qId);
}

void UWidgetCanvasWorld::OpenEnchant()
{
	m_Enchant->Open();
}

void UWidgetCanvasWorld::PrintErrorText(const FString& string)
{
	m_AlertInfoWindow->PrintErrorText(string);
}

void UWidgetCanvasWorld::ShowZone(const FText& nameT)
{
	m_TextZoneName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	PlayAnimation(ZoneOpen);

	m_TextZoneName->SetText(nameT);
}

void UWidgetCanvasWorld::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);
	
	if(Player.GetAnimation() == ZoneOpen)
	{
		m_TextZoneName->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(Player.GetAnimation() == LevelUp)
	{
		m_WindowLevelUp->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if(Player.GetAnimation() == GameOver)
	{
		//m_WindowGameOver->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UWidgetEquipInvenPanel* UWidgetCanvasWorld::GetEquipInvenMenu()
{
	return m_EquipInvenPanel;
}

UWidgetPickpocketPanel* UWidgetCanvasWorld::GetPickpocketMenu()
{
	return m_PickpocketPanel;
}

UWidgetConfirmPanel* UWidgetCanvasWorld::GetConfirmPanel()
{
	return m_ConfirmPanel;
}

void UWidgetCanvasWorld::OpenItemInfoData(const FItemDataRow& item)
{
	m_ItemInfo->SetInfoItemData(item);
}

void UWidgetCanvasWorld::OpenItemInfo(FItemSpec& item, UInventory* inven)
{
	m_ItemInfo->SetItemInfo(item,inven);
}

void UWidgetCanvasWorld::OpenItemInfoCollec(const FName& collecId, int collecIndex, const FName& itemID)
{
	m_ItemInfo->SetCollecItemInfo(collecId,collecIndex,itemID);
}

void UWidgetCanvasWorld::HideHUD()
{
	m_BtnMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetCanvasWorld::ShowHUD()
{
	m_BtnMenu->SetVisibility(ESlateVisibility::Visible);
}

FReply UWidgetCanvasWorld::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UWidgetCanvasWorld::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void UWidgetCanvasWorld::ShowLevelUpWindow()
{
	m_WindowLevelUp->SetVisibility(ESlateVisibility::HitTestInvisible);

	int Level = UMyGameInstance::Get->m_PlayerStatManager->GetLevel();

	m_WindowLevelUp->SetLevel(Level);

	PlayAnimation(LevelUp);
}

void UWidgetCanvasWorld::ShowGameOverWindow(const ACombatUnitPawn* killer)
{
	m_WindowGameOver->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_WindowGameOver->SetKiller(killer);

	PlayAnimation(GameOver);
}

void UWidgetCanvasWorld::StartPickPocket(AMonsterPawn* mob)
{
	m_PickpocketPanel->SetTargetPawn(mob);
}

void UWidgetCanvasWorld::ShowMainHUD(bool b)
{
	if(b)
	{
		m_MainHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_MainHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UWidgetStorage* UWidgetCanvasWorld::GetStorageMenu()
{
	return m_StoragePanel;
}

void UWidgetCanvasWorld::OpenItemConverter(UItemConvertInst* inst)
{
	m_ItemConvertPanel->ShowItemConvert(inst);
}

UWidgetWaitInteract* UWidgetCanvasWorld::GetWaitInteract()
{
	return m_WaitBar;
}

UWidgetScreenEffect* UWidgetCanvasWorld::GetScreenEffect()
{
	return m_ScreenEffect;
}

void UWidgetCanvasWorld::OpenAssemble(UAssembleInst* assemble_inst)
{
	m_ItemAssemblePanel->ShowAssemble(assemble_inst);
}

void UWidgetCanvasWorld::OpenRidePanel(UInventory* inven)
{
	m_RidePanel->ShowRidePanel(inven);
}

void UWidgetCanvasWorld::OnZoneMove(const FName& zoneID)
{
	if(zoneID == TEXT("PlayerHome"))
	{
		m_BtnBuild->OnClicked.AddDynamic(this,&UWidgetCanvasWorld::OpenBuild);

		m_BtnBuild->SetVisibility(ESlateVisibility::Visible);

		m_bIsBuildable = true;
	}
	else
	{
		m_BtnBuild->SetVisibility(ESlateVisibility::Collapsed);

		m_bIsBuildable = false;
	}
}
