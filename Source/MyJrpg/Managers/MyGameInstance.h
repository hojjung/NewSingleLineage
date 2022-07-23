// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "CollectionManager.h"
#include "CraftManager.h"
#include "CurrencyManager.h"
#include "EnchantManager.h"
#include "EventStageManager.h"
#include "FriendKarmaManager.h"
#include "ItemConvertManager.h"
#include "LevelMoveManager.h"
#include "ParticleEffectManager.h"
#include "PlayerStatusManager.h"
#include "QuestManager.h"
#include "QuickSlotManager.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "SpawnEventManager.h"
#include "ZoneMoveManager.h"
#include "Build/ZoneInstManager.h"
#include "Engine/GameInstance.h"
#include "MyJrpg/Actors/MyPlayerController.h"
#include "MyJrpg/DataTables/BadwordTable.h"
#include "MyJrpg/GameRules/GameRuleBase.h"
#include "MyJrpg/Pawns/Logics/PlSkillAuto.h"
#include "Web/ChatManager.h"
#include "Web/PlayfabManager.h"
#include "MyGameInstance.generated.h"

class UPreviewActorManager;
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
	UPlayfabManager* m_PlayfabManager;
	UPROPERTY()
	ULevelMoveManager* m_LevelMoveManager;
	UPROPERTY()
	UPlayerStatusManager* m_PlayerStatManager;
	UPROPERTY()
	URewardManager* m_RewardManager;
	UPROPERTY()
	UInventory* m_Inven;
	
	UPROPERTY()
	UEquipManager* m_EquipManager;
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
	UEnchantManager* m_EnchantManager;
	UPROPERTY()
	UCollectionManager* m_ItemCollecManager;
	UPROPERTY()
	UPetManager* m_PetManager;
	UPROPERTY()
	UFriendKarmaManager* m_TeamKarma;
	UPROPERTY()
	UConstructionManager* m_BuildManager;
	UPROPERTY()
	UQuickSlotManager* m_QuickManager;
	UPROPERTY()
	UPreviewActorManager* m_PreviewActorManager;
	UPROPERTY()
	UItemConvertManager* m_ItemConvertManager;
	UPROPERTY()
	UZoneInstManager* m_ZoneInst;
	UPROPERTY()
	UZoneMoveManager* m_ZoneMove;
	UPROPERTY()
	USpawnEventManager* m_SpawnEvent;
	UPROPERTY()
	UEventStageManager* m_EventStage;
	
public:
	TWeakObjectPtr<AMyPlayerPawn> m_Player;
	
	TWeakObjectPtr<AMyPlayerController> m_PlayerCon;

protected:
	virtual void BeginDestroy() override;
	
	void IterateItemTableToRegister();

	void TestModeSetting();
	
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
