#pragma once

#include "CoreMinimal.h"
#include "MyJrpg/DataTables/UnitEntityData.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBaseElement.h"
#include "WidgetZoneMonsterElement.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetZoneMonsterElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UWidgetBaseElement* m_ElementBase;

protected:
	void OnHoldingComplete();
public:
	void SetUnit(const FNpcUnitEntityRow* npc_entity);
};
