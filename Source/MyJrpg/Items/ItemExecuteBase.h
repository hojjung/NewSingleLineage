// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "ItemExecuteBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MYJRPG_API UItemExecuteBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void Use(const FName& item) const {};
};
