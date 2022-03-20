// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "NPCPaletteDataAsset.h"
#include "UObject/NoExportTypes.h"
#include "FieldPropData.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UFieldPropData : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FFieldPropDataRow : public FEntityRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<AActor> m_ClassUnit;
};