#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "NPCPaletteDataAsset.generated.h"


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEntityRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true"))
	FText m_Desc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_Icon;
};

USTRUCT()
struct FNPCSpawnData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="NPCPalette")
	FName m_IDEntity;
	UPROPERTY(EditAnywhere, Category="NPCPalette")
	FVector  m_SpawnPosition;
	UPROPERTY(EditAnywhere, Category="NPCPalette")	
	FRotator m_SpawnRotation;

	UPROPERTY(EditAnywhere, Category="NPCPalette")
	UDataTable* m_EntityParentTable;
};

UCLASS(BlueprintType, hidecategories=(Object))
class NPCPALETTE_API UNPCPaletteDataAsset : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="NPCPalette")
	TArray<FNPCSpawnData> m_ArySpawnDatas;
};
