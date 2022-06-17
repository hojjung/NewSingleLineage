#include "MyGameInstance.h"
#include "ConstructionManager.h"
#include "EquipManager.h"
#include "MyAssetManager.h"
#include "RewardManager.h"
#include "PetManager.h"
#include "PreviewActorManager.h"
#include "GameFramework/GameUserSettings.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"


UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	Get = nullptr;
	
	m_Player = nullptr;

	m_PlayerCon = nullptr;
}

void UMyGameInstance::IterateItemTableToRegister()
{
	TArray<const FItemDataRow *> AryItemRows;
	
	UItemData::GetItemTable->GetAllRows("",AryItemRows);
	TArray<FName> AryItemNames =  UItemData::GetItemTable->GetRowNames();

	int Iter = -1;
	while (++Iter < AryItemRows.Num())
	{
		const auto& DropDatas = AryItemRows[Iter]->m_AryDropDatas;
		
		if(DropDatas.Num() > 0)
		{
			for(const auto& Drop : DropDatas)
			{
				m_RewardManager->AddDropItemData(Drop, AryItemNames[Iter]);
			}
		}

		const auto& CraftDatas = AryItemRows[Iter]->m_AryCostItem;

		if(CraftDatas.Num() > 0)
		{
			m_CraftManager->AddCraftItemData(AryItemNames[Iter]);
		}

		const auto& TradeDatas = AryItemRows[Iter]->m_AryTraderIDs;

		if(TradeDatas.Num() > 0)
		{
			for(const auto& Trade : TradeDatas)
			{
				m_ShopManager->AddTradeItemData(Trade, AryItemNames[Iter]);
			}
		}
	}
}


void UMyGameInstance::Init()
{
	Super::Init();
	
	Get = this;

	m_EffectManager = NewObject<UParticleEffectManager>(this);
	m_LevelMoveManager = NewObject<ULevelMoveManager>(this);
	m_PlayerStatManager = NewObject<UPlayerStatusManager>(this);
	m_RewardManager = NewObject<URewardManager>(this);
	m_EquipManager = NewObject<UEquipManager>(this);
	m_CurrencyManager= NewObject<UCurrencyManager>(this);
	m_CraftManager = NewObject<UCraftManager>(this);
	m_QuestManager = NewObject<UQuestManager>(this);
	m_ShopManager = NewObject<UShopManager>(this);
	m_SkillManager = NewObject<USkillManager>(this);
	m_Inven = NewObject<UInventory>(this);
	m_ItemConvertManager = NewObject<UItemConvertManager>(this);
	m_EnchantManager = NewObject<UEnchantManager>(this);
	
	m_BadwordTable =NewObject<UBadwordTable>(this);
	m_ChatManager = NewObject<UChatManager>(this);

	m_SkillAuto = NewObject<UPlSkillAuto>(this);

	m_ItemCollecManager = NewObject<UCollectionManager>(this);

	m_PetManager = NewObject<UPetManager>(this);

	m_TeamKarma = NewObject<UFriendKarmaManager>(this);

	m_BuildManager = NewObject<UConstructionManager>(this);

	m_QuickManager = NewObject<UQuickSlotManager>(this);

	m_PreviewActorManager = NewObject<UPreviewActorManager>(this);

	m_ZoneInst = NewObject<UZoneInstManager>(this);

	m_BuildManager->Init();
	m_ItemCollecManager->Init();
	m_PetManager->Init();
	m_SkillManager->Init();
	m_EquipManager->Init();
	m_Inven->Init(FGlobalVariable::INVEN_SIZE,NSLOCTEXT("UMyGameInstance","Inven","소지품"));
	m_QuestManager->Init();
	m_CurrencyManager->Init(0);
	m_PlayerStatManager->Init();
	m_BadwordTable->Init();
	m_TeamKarma->Init();
	//
	IterateItemTableToRegister();

	m_CraftManager->Init();
	
	m_AryStorage.Reset();

	if(UMyLib::IsTestMode())
	{
		PRINTF("TestMode: True");
		TestModeSetting();
	}
	else
	{
		PRINTF("TestMode: False");
	}
}

void UMyGameInstance::TestModeSetting()
{
	m_Inven->AddItem(FItemSpec(TEXT("Wood01"),1));
}

void UMyGameInstance::StartGame()
{	//이런식으로 로드가 추가되면 됨
	UKismetSystemLibrary::ControlScreensaver(false);
	
	m_LevelMoveManager->StartGame();
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	if(m_LevelMoveManager->IsGameStart())
	{
		m_LevelMoveManager->OnOpenWorldLevelComplete();
	}
	else
	{
		StartGame();
	}
}

FString UMyGameInstance::GetPlayerName()
{
	return TEXT("Need Set Name");
}

void UMyGameInstance::Tick(float deltaTime)
{
	m_PlayerStatManager->Tick(deltaTime);
	m_SkillManager->Tick(deltaTime);
	m_ChatManager->Tick(deltaTime);
	m_SkillAuto->Tick(deltaTime);
	m_ItemConvertManager->Tick(deltaTime);
}

void UMyGameInstance::AddStorage(UInventory* inven)
{
	m_AryStorage.Add(inven);
}

void UMyGameInstance::RemoveStorage(UInventory* inven)
{
	m_AryStorage.Remove(inven);
}

TArray<UInventory*>& UMyGameInstance::GetStorages()
{
	return m_AryStorage;
}
