// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShopManager.generated.h"

/**
 * 상점은 판매가 가능해야한다
 */
UCLASS()
class MYJRPG_API UShopManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDead,FText);

	FOnMonsterDead m_OnItemBought;

protected:
	TMap<FName,TArray<FName>> m_MapTraderItems;
	
public:
	void BuyItem(const FName& itemID, int amount);
	
	void SellItem(const FName& id, int amount);
	
	void AddTradeItemData(const FName& traderID, const FName& itemID);
	
	const TArray<FName>& GetShopItems(const FName& traderID);
};
