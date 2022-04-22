#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/MyJrpg.h"
#include "UObject/NoExportTypes.h"
#include "FriendKarmaManager.generated.h"

class ACombatUnitPawn;
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

	bool IsFoe(const ACombatUnitPawn* Other);

	EKarma GetUnitKarma(const ACombatUnitPawn* Other);
};
