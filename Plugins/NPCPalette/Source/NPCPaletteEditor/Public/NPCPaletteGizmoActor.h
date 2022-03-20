// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "SNPCPaletteModeTool.h"
#include "Components/ArrowComponent.h"
#include "Engine/NavigationObjectBase.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Components/WidgetComponent.h"
#include "NPCPaletteGizmoActor.generated.h"

class UTestUWidget;
class STextBlock;

class SImage;




UCLASS(Blueprintable,Transient, ClassGroup=Common, hidecategories=Collision)
class NPCPALETTEEDITOR_API ANPCPaletteGizmoActor : public ANavigationObjectBase
{
	GENERATED_BODY()

public:
	ANPCPaletteGizmoActor(const FObjectInitializer& ObjectInitializer);

	FNPCPaletteItem m_CurrentNPC;
	UPROPERTY()
	UArrowComponent* m_ArrowComponent;
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* m_WidgetComp;
	UPROPERTY(VisibleAnywhere)
	FName m_RowID;

	void SetTextIcon(FText,UTexture2D*);
	
	UArrowComponent* GetArrowComponent() const;
};


