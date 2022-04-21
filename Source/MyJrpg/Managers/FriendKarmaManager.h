#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FriendKarmaManager.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UFriendKarmaManager : public UObject
{
	GENERATED_BODY()

protected:
	TMap<FName, int> m_MapKarma;

public:
	void Init();

	int GetKarma(const FName&& id);
	
	int GetKarma(const FName& id);
};
