
#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/Items/Inventory.h"
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
	virtual void Use(FItemSpec& item,UInventory* inven) const {};
};
