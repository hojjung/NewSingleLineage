// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CurrencyManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UCurrencyManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnGoldChanged);

	FOnGoldChanged m_GoldChanged;
	
protected:
	int64 m_nGold;
	
public:
	void Init(int gold);
	
	int GetGold();

	bool SubGold(int amount);

	void AddGold(int amount);

	bool CheckGoldEnough(int amount);
};
