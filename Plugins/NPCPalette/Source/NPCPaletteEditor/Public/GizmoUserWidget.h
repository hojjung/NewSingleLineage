// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GizmoUserWidget.generated.h"

/**
*     static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WorldWidget/WB_DamageText.WB_DamageText_C'"));
    
 */

class UTextBlock;
class UImage;

UCLASS()
class NPCPALETTEEDITOR_API UGizmoUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_Image;

public:
	void SetImageAndText(UTexture2D* tt2D,FText tt);
};
