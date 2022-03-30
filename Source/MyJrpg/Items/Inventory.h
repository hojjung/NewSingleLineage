// All right Reserve 2021 HereticByte

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/QuestData.h"
#include "MyJrpg/Managers/EnchantManager.h"
#include "Inventory.generated.h"

/**
 * 
 */


UCLASS()
class MYJRPG_API UInventory : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE(FOnInvenChanged);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemChanged,const FName&);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemObtained,const FItemDataRow&, int);

	FOnItemObtained m_OnItemObtain;

	FOnInvenChanged m_OnInvenChanged;

	FOnItemChanged m_OnNewItemAdded;

	FOnItemChanged m_OnNewItemRemoved;

protected://
	typedef TMap<FName, int> TEquipLevelPair;
	
	TArray<FName> m_AryTotalItems;//전체 아이템의 칸 순서
	
	TMap<FName, int> m_MapMiscItems;//기타 아이템과 개수

	TMap<FName, TEquipLevelPair> m_MapEquipItemIdGroup;//롱소드 : 롱소드01=1, 롱소드02=4

	int m_nInvenMaxSize;

public:
	bool IsCountAvailable();
	
	int GetInvenSize();
	
	void Init(int size);
	
	int GetAvailalbeStackCount(FName id);

	bool AddItem(FName id,int amount = 1);

	bool AddEquipItem(FName gid, int lv = 0);

	void RemoveItem(FName id,int amount);//amount = - 1 remove whole

	void RemoveEquipItem(FName gid);

	bool IsEquipItem(FName hasID);

	int GetItemStack(FName ID);

	int GetItemLevel(FName gID);

	void AddItemLevel(FName gID, int addlv);

	void SubItemLevel(FName gID, int sublv);

	const TArray<FName>& GetAryTotalItemIDs() const;

	FName GetItemID(int index);
	
	bool FindMisItem(const FName& name, int amount =1);
	
	int GetUsingSlotCount();
	
	const FName* FindEquipItem(const FName& Oid, int lv);

	const FName* FindEquipItem(const FName& Oid);
};

