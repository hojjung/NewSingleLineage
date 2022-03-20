// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "NavigationData.h"
#include "MyJrpg/Save/MySaveGame.h"
#include "UObject/NoExportTypes.h"
#include "LevelMoveManager.generated.h"

struct FDropDataRow;
struct FZoneDataRow;
/**
 * 
 */
UCLASS()
class MYJRPG_API ULevelMoveManager : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelMoveComplete,const FText&);

	FOnLevelMoveComplete m_OnLvelMoveComp;
	
protected:
	const FZoneDataRow* m_ZoneData;

	bool m_bIsGameStart = false;

	int m_nStageIndex;
public:
	void StartGame();

	void OpenMyLevel(const FZoneDataRow& zoneData, int index);

	void OpenMyLevel(FName zoneData, int index);

	void OnOpenWorldLevelComplete();
	
	bool IsGameStart();
	
	void HideCurrentLevel();

	void ShowCurrentLevel();

	const FZoneDataRow* GetZoneDataCurrent() const;

	const FDropDataRow* GetDropData() const;
};

