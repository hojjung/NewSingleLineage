// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "ItemData.h"
#include "UObject/NoExportTypes.h"
#include "ShopData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UShopData : public UObject
{
	GENERATED_BODY()
public:
	UShopData();
	
	static UDataTable* GetShopTable;

	static UDataTable* GetBlackSmithTable;
};

USTRUCT(BlueprintType)//플레이어가 살 수있는 품목? = 잠금해제 방식?
struct FItemTradingData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDataHandle m_ItemDataRowHandle;

public:
	TSoftObjectPtr<UTexture2D> GetItemIcon() const
	{
		if(m_ItemDataRowHandle.IsNull() || m_ItemDataRowHandle.RowName.IsNone())
		{
			return nullptr;
		}
		
		return m_ItemDataRowHandle.GetRow<FItemDataRow>("")->m_ItemIcon;
	}
	
	int GetCost() const
	{
		if(m_ItemDataRowHandle.IsNull() || m_ItemDataRowHandle.RowName.IsNone())
		{
    		return -1;
    	}
    	return m_ItemDataRowHandle.GetRow<FItemDataRow>("")->m_nBuyValue;
	}
};