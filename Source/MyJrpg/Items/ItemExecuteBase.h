
#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "ItemExecuteBase.generated.h"

class UInventory;
/**
 * 
 */
UCLASS(Abstract)
class MYJRPG_API UItemExecuteBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void Use(const FName& item,UInventory* inven) const {};
};
