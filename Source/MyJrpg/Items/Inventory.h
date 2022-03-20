// All right Reserve 2021 HereticByte

#pragma once

#include "MyJrpg/MyJrpg.h"
#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/QuestData.h"
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

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewItemChanged,FName);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipItemChanged,const FItemSpec&);

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemObtained,const FItemDataRow&, int);

	FOnInvenChanged m_OnInvenChanged;

	FOnNewItemChanged m_OnNewItemAdded;//아예 새로 얻는 아이템

	FOnNewItemChanged m_OnNewItemRemoved;//아예 없어지는 아이템,스텍 <=0

	FOnItemObtained m_OnItemObtain;

	enum class EResult
	{
		Success,
		FailMaxCount,//count max for equip
		FailStackCount
	};
public:
	UInventory();

	int m_nInvenMaxSize=0;
protected://
	TArray<FItemSpec> m_AryItems;

protected:
	bool IsItemWillBeZeroOnErase(FName id,int amount=1);//0까지 오케이
	
public:
	void Init(int size);

	bool CheckEmptySlot(int needCount);

	bool CheckEmptyStack(FName id,int needCount);

	int GetStackableCount(FName id);

	int GetItemIndex(FName id);
	
	FItemSpec* GetItem(FName id);
	
	EResult AddItem(FName id,int& amount,int level=0);

	EResult AddItemInst(const FItemSpec& itemOld);//몬스터와 상점에서 신품 얻기,창고에서 고품 얻기

	int RemoveItem(FName id,int amount=1);//Remain

	void RemoveItem(const FItemSpec& itemOld);

	FORCEINLINE const TArray<FItemSpec>& GetAllItems() const
	{
		return  m_AryItems;
	}

	void LevelUpEquipItem(const FItemSpec& itemOld);

	int GetItemAmount(FName id);

	int GetTotalItemCount();

	int GetEmptyIndex();

	bool IsInvenHasSpace(const FItemDataRow& crafting_data,int amount);

	bool CheckHasItem(FName key, int count=1);

	const FItemSpec& GetItem(int index) const;
};

