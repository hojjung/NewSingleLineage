// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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

USTRUCT(BlueprintType)
struct FZone
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_ZoneUniqueID;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_MapName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UNPCPaletteDataAsset* m_SpawnDataNpc;
};

USTRUCT(BlueprintType)//Key is ZoneName,OpenLevel
struct FZoneDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZoneDataRow();

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameRuleBase> m_ClassGameRule;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine="true"))
	FText m_Desc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FZone> m_AryZones;
	//즉 상호작용하되 안움직이는게 존재하는것
	//그럼 상호작용 기능을 어떻게 분리할것인가
};
