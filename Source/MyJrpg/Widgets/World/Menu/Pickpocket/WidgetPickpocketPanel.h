#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Pawns/MonsterPawn.h"
#include "MyJrpg/Widgets/World/CommonElements/WidgetBasePanel.h"
#include "MyJrpg/Widgets/World/Menu/Inventory/WidgetInventory.h"
#include "MyJrpg/Widgets/World/Menu/Storage/WidgetStorage.h"
#include "WidgetPickpocketPanel.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPickpocketPanel : public UWidgetStorage
{
	GENERATED_BODY()
	
	TWeakObjectPtr<AMonsterPawn> m_TargetPawn;
	
public:
	void SetTargetPawn(AMonsterPawn* targetPawn);
	
	AMonsterPawn* GetCurrentTargetPawn();

protected:
	virtual void OnWithdrawAll() override;
};
