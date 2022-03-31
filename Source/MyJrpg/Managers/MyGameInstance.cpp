#include "MyGameInstance.h"
#include "MyGameInstance.h"
#include "EquipManager.h"
#include "MyAssetManager.h"
#include "RewardManager.h"
#include "PetManager.h"
#include "GameFramework/GameUserSettings.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Items/Inventory.h"


UMyGameInstance* UMyGameInstance::Get = nullptr;

void UMyGameInstance::BeginDestroy()
{
	Super::BeginDestroy();

	Get = nullptr;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	Get = this;

	m_EffectManager = NewObject<UParticleEffectManager>(this);
	m_LevelMoveManager = NewObject<ULevelMoveManager>(this);
	m_SpawnManager = NewObject<USpawnManager>(this);
	m_PlayerStatManager = NewObject<UPlayerStatusManager>(this);
	m_RewardManager = NewObject<URewardManager>(this);
	m_EquipManager = NewObject<UEquipManager>(this);
	m_ItemExeManager = NewObject<UItemExecuteManager>(this);
	m_CurrencyManager= NewObject<UCurrencyManager>(this);
	m_CraftManager = NewObject<UCraftManager>(this);
	m_QuestManager = NewObject<UQuestManager>(this);
	m_ShopManager = NewObject<UShopManager>(this);
	m_SkillManager = NewObject<USkillManager>(this);
	m_Inven = NewObject<UInventory>(this);
	m_PotionManager = NewObject<UPotionManager>(this);
	m_EnchantManager = NewObject<UEnchantManager>(this);
	
	m_BadwordTable =NewObject<UBadwordTable>(this);
	m_ChatManager = NewObject<UChatManager>(this);

	m_AvatarManager = NewObject<UAvatarManager>(this);
	m_SkillAuto = NewObject<UPlSkillAuto>(this);

	m_ItemCollecManager = NewObject<UCollectionManager>(this);

	m_PetManager = NewObject<UPetManager>(this);

	m_AryStorage.Reset();
	m_AryStorage.Add(NewObject<UInventory>(this));
	m_AryStorage.Add(NewObject<UInventory>(this));
	m_AryStorage.Add(NewObject<UInventory>(this));
	m_AryStorage.Add(NewObject<UInventory>(this));
	m_AryStorage[0]->Init(FGlobalVariable::STORAGE_SIZE);
	m_AryStorage[1]->Init(FGlobalVariable::STORAGE_SIZE);
	m_AryStorage[2]->Init(FGlobalVariable::STORAGE_SIZE);
	m_AryStorage[3]->Init(FGlobalVariable::STORAGE_SIZE);


	m_ItemCollecManager->Init();
	m_AvatarManager->Init();
	m_PetManager->Init();
	m_PotionManager->Init();
	m_SkillManager->Init();
	m_Inven->Init(FGlobalVariable::INVEN_SIZE);
	m_QuestManager->Init();
	m_CurrencyManager->Init(0);
	m_CraftManager->InitLoadAllCraftItemData();
	m_ItemExeManager->Init();
	m_PlayerStatManager->Init();
	m_EquipManager->Init();
	m_BadwordTable->Init();
}

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	if(MapName!=TEXT("InitLevel"))
	{
		if(m_LevelMoveManager->IsGameStart())
		{
			m_SkillAuto->Init();
			m_LevelMoveManager->OnOpenWorldLevelComplete();

		}
	}
}

void UMyGameInstance::StartGame()
{	//이런식으로 로드가 추가되면 됨
	m_LevelMoveManager->StartGame();

	UKismetSystemLibrary::ControlScreensaver(false);
}

FString UMyGameInstance::GetPlayerName()
{
	return TEXT("Need Set Name");
}

void UMyGameInstance::Tick(float deltaTime)
{
	m_SkillManager->Tick(deltaTime);
	m_ChatManager->Tick(deltaTime);
	m_SkillAuto->Tick(deltaTime);
	m_PotionManager->Tick(deltaTime);
}

UInventory* UMyGameInstance::GetStorage()
{
	return m_AryStorage[m_nStorageIndex];
}

void UMyGameInstance::SelectStorage(int index)
{
	m_nStorageIndex = index;
}