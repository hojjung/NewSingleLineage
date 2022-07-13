// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyJrpg/MyJrpg.h"
#include "UObject/NoExportTypes.h"
#include "ZoneData.generated.h"

class UGameRuleBase;
class UNPCPaletteDataAsset;
struct FNPCSpawnData;
/**
	존의 정보 그자체
	존과 존이동 때문에 필요.
	해당 존이 실내인지 실외인지 = 실내이동 이라면 밖의 존을 하이드,실외라면 전부 메모리 해제
 */

UCLASS()
class MYJRPG_API UZoneData : public UObject
{
	GENERATED_BODY()
public:
	UZoneData();
	
	static UDataTable* GetZoneTable;
};

USTRUCT(BlueprintType)//Key is ZoneName,OpenLevel
struct FZoneDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_RowKey = TEXT("Should Same Row Key");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bNeedBoat = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool m_bNeedHorse = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine="true"))
	FText m_Desc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_MapName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_SpawnEventID;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UNPCPaletteDataAsset* m_SpawnDataNpc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nLevelMob = 1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nLevelWood = 1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nLevelRock = 1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int m_nLevelItem = 1;
};
