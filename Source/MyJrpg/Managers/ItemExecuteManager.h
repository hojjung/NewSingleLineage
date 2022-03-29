// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "MyJrpg/DataTables/ItemData.h"
#include "MyJrpg/DataTables/SkillData.h"
#include "UObject/NoExportTypes.h"
#include "ItemExecuteManager.generated.h"

struct FItemDataRow;
class UItemExecuteBase;
class UConsumeItemBase;
/**
 * 
 */
UCLASS()
class MYJRPG_API UItemExecuteManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemExe,FText);

	FOnItemExe m_OnItemUse;

protected:
	TMap<FName,UItemExecuteBase*> m_MapItems;

	UPROPERTY()
	TArray<UItemExecuteBase*> m_AryItems;
	
public:
	void Init();
	
	void AddUseItem(const FName& itemKey);

	void RemoveItem(const FName& itemKey);
//
	void EquipUnequipItem(const FName& itemSpec);

	void ExecuteItem(const FName& itemID,UInventory* inven);
};