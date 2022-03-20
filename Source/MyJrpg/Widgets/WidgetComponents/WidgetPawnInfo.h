// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "WidgetPawnInfo.generated.h"

class ACombatUnitPawn;
/**
 * 
 */
UCLASS()
class MYJRPG_API UWidgetPawnInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextPawnName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* m_HpBar;

public:
	void SetPawnInfo(const ACombatUnitPawn* pawn);
};
