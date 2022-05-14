// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "AvatarManager.h"
#include "CollectionManager.h"
#include "CraftManager.h"
#include "CurrencyManager.h"
#include "EnchantManager.h"
#include "FriendKarmaManager.h"
#include "ItemExecuteManager.h"
#include "LevelMoveManager.h"
#include "ParticleEffectManager.h"
#include "PlayerStatusManager.h"
#include "PotionManager.h"
#include "QuestManager.h"
#include "QuickSlotManager.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "SpawnManager.h"
#include "Engine/GameInstance.h"
#include "MyJrpg/DataTables/BadwordTable.h"
#include "MyJrpg/GameRules/GameRuleBase.h"
#include "MyJrpg/Pawns/Logics/PlSkillAuto.h"
#include "Web/ChatManager.h"
#include "MyGameInstance.generated.h"

class UConstructionManager;
class UEquipManager;
class URewardManager;
class UPetManager;
/**
 * 
 */
UCLASS()
class MYJRPG_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	static UMyGameInstance* Get;

protected:
	UPROPERTY()
	TArray<UInventory*> m_AryStorage;
	
public:
	UPROPERTY()
	ULevelMoveManager* m_LevelMoveManager;
	UPROPERTY()
	USpawnManager* m_SpawnManager;
	UPROPERTY()
	UPlayerStatusManager* m_PlayerStatManager;
	UPROPERTY()
	URewardManager* m_RewardManager;
	UPROPERTY()
	UInventory* m_Inven;
	
	UPROPERTY()
	UEquipManager* m_EquipManager;
	UPROPERTY()
	UItemExecuteManager* m_ItemExeManager;
	UPROPERTY()
	UCurrencyManager* m_CurrencyManager;
	UPROPERTY()
	UCraftManager* m_CraftManager;
	UPROPERTY()
	UQuestManager* m_QuestManager;
	UPROPERTY()
	UShopManager* m_ShopManager;
	UPROPERTY()
	USkillManager* m_SkillManager;
	UPROPERTY()
	UParticleEffectManager* m_EffectManager;
	UPROPERTY()
	UBadwordTable* m_BadwordTable;
	UPROPERTY()
	UChatManager* m_ChatManager;
	UPROPERTY()
	UPlSkillAuto* m_SkillAuto;
	UPROPERTY()
	UPotionManager* m_PotionManager;
	UPROPERTY()
	UEnchantManager* m_EnchantManager;
	UPROPERTY()
	UAvatarManager* m_AvatarManager;
	UPROPERTY()
	UCollectionManager* m_ItemCollecManager;
	UPROPERTY()
	UPetManager* m_PetManager;
	UPROPERTY()
	UGameRuleBase* m_GameRule;
	UPROPERTY()
	UFriendKarmaManager* m_TeamKarma;
	UPROPERTY()
	UConstructionManager* m_BuildManager;
	UPROPERTY()
	UQuickSlotManager* m_QuickManager;
	
public:
	UPROPERTY()
	AMyPlayerPawn* m_Player;
	UPROPERTY()
	AMyPlayerController* m_PlayerCon;
	
protected:
	virtual void BeginDestroy() override;
	
	void IterateItemTableToRegister();
	
public:
	virtual void Init() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	void StartGame();

	FString GetPlayerName();

	void Tick(float deltaTime);

	void AddStorage(UInventory* inven);

	void RemoveStorage(UInventory* inven);

	TArray<UInventory*>& GetStorages();
};
