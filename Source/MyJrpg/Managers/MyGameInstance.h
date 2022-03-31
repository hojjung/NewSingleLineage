// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "AvatarManager.h"
#include "CollectionManager.h"
#include "CraftManager.h"
#include "CurrencyManager.h"
#include "EnchantManager.h"
#include "ItemExecuteManager.h"
#include "LevelMoveManager.h"
#include "ParticleEffectManager.h"
#include "PlayerStatusManager.h"
#include "PotionManager.h"
#include "QuestManager.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "SpawnManager.h"
#include "Engine/GameInstance.h"
#include "MyJrpg/DataTables/BadwordTable.h"
#include "MyJrpg/Pawns/Logics/PlSkillAuto.h"
#include "Web/ChatManager.h"
#include "MyGameInstance.generated.h"

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
	TArray<UInventory*> m_AryStorage;
	UPROPERTY()
	int m_nStorageIndex;
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
	
protected:
	virtual void BeginDestroy() override;
	
public:
	virtual void Init() override;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	
	void StartGame();

	FString GetPlayerName();

	void Tick(float deltaTime);

	UInventory* GetStorage();

	void SelectStorage(int index);
};
