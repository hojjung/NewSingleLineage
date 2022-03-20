// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "PortraitData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UPortraitData : public UObject
{
	GENERATED_BODY()

public:
	UPortraitData();
	
	static UDataTable* GetPortraitTable;
	
};

USTRUCT(BlueprintType)
struct FPortraitRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> m_IconPortrait;
};