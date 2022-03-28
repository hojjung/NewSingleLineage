// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/ItemCollectionTable.h"
#include "MyJrpg/Items/Inventory.h"
#include "UObject/NoExportTypes.h"
#include "CollectionManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UCollectionManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnCollecChanged);

	FOnCollecChanged m_OnCollecChanged;

protected:
	typedef TArray<bool> TUnlockedItems;

	typedef TArray<int> TOptionGroups;
	
	int m_nCompleteCount;

	TArray<FName> m_AryItemKeys;
	
	TArray<FItemCollecRow*> m_AryItemRows;

	TMap<FName, TUnlockedItems> m_MapCollecSpec;//각 콜렉션별 저장사항

	TMap<TSubclassOf<UOptionBase>, TOptionGroups> m_MapTotalStats;//각 스텟별,이를 이용해 실제 스텟적용도 하게될듯
	
protected:
	int GetTotalCount();

	void CalculateComplete();

	int GetCompleteCount();
	
public:
	void Init();

	const TArray<FItemCollecRow*>& GetAryCollec() const;

	const TArray<FName>& GetAryCollecKeys() const;

	const TMap<FName,TArray<bool>>& GetMapCollecSpecs() const;

	const TMap<TSubclassOf<UOptionBase>, TOptionGroups>& GetMapOptionGroups() const ;

	FText GetTotalProgressText();

	float GetTotalProgress();

	bool CheckCanAdd(FName collectionID, int indexItem, UInventory* invenFrom);

	void AddItem(FName collectionID, int indexItem, UInventory* invenFrom);
};
