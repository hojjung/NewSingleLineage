#include "NPCPaletteGizmoActor.h"
#include "GizmoUserWidget.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

ANPCPaletteGizmoActor::ANPCPaletteGizmoActor(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 92.0f);
	GetCapsuleComponent()->SetShouldUpdatePhysicsVolume(false);

	m_ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	 
	
	if (!IsRunningCommandlet())
	{
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> PlayerStartTextureObject;
			FName ID_PlayerStart;
			FText NAME_PlayerStart;
			FName ID_Navigation;
			FText NAME_Navigation;//Engine/EditorResources/S_Player
			FConstructorStatics()//Texture2D'/NPCPalette/Icon128.Icon128'
				: PlayerStartTextureObject(TEXT("Texture2D'/NPCPalette/Icon128.Icon128'"))
				, ID_PlayerStart(TEXT("PlayerStart"))
				, NAME_PlayerStart(NSLOCTEXT("SpriteCategory", "PlayerStart", "Player Start"))
				, ID_Navigation(TEXT("Navigation"))
				, NAME_Navigation(NSLOCTEXT("SpriteCategory", "Navigation", "Navigation"))
			{}//
		};
		static FConstructorStatics ConstructorStatics;

		if (GetGoodSprite())
		{
			//GetGoodSprite()->Sprite = ConstructorStatics.PlayerStartTextureObject.Get();
			GetGoodSprite()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
			GetGoodSprite()->SpriteInfo.Category = ConstructorStatics.ID_PlayerStart;
			GetGoodSprite()->SpriteInfo.DisplayName = ConstructorStatics.NAME_PlayerStart;
		}
		if (GetBadSprite())
		{
			GetBadSprite()->SetVisibility(false);
		}

		if (m_ArrowComponent)
		{
			m_ArrowComponent->ArrowColor = FColor(150, 200, 255);

			m_ArrowComponent->ArrowSize = 1.0f;
			m_ArrowComponent->bTreatAsASprite = true;
			m_ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Navigation;
			m_ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Navigation;
			m_ArrowComponent->SetupAttachment(GetCapsuleComponent());
			m_ArrowComponent->bIsScreenSizeScaled = true;
		}
	}

	m_WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComp01");
	m_WidgetComp->SetupAttachment(RootComponent);
	m_WidgetComp->SetRelativeLocation(FVector(0, 0, 55));
	m_WidgetComp->SetVisibility(true);
	m_WidgetComp->SetDrawAtDesiredSize(true);
	m_WidgetComp->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(TEXT("WidgetBlueprint'/NPCPalette/WB_NPCGizmo.WB_NPCGizmo_C'"));
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMat(TEXT("Material'/NPCPalette/MAT_Billboard.MAT_Billboard'"));

	//200 40

	m_WidgetComp->EmptyOverrideMaterials();
	m_WidgetComp->SetMaterial(0,FoundMat.Object);
	m_WidgetComp->SetWidgetClass(FoundW.Class);
	m_WidgetComp->SetDrawAtDesiredSize(false);
	m_WidgetComp->SetDrawSize(FVector2D(200.f,40.f));
	m_WidgetComp->UpdateWidget();
}

/** Returns ArrowComponent subobject **/
UArrowComponent* ANPCPaletteGizmoActor::GetArrowComponent() const { return m_ArrowComponent; }

void ANPCPaletteGizmoActor::SetTextIcon(FText tt, UTexture2D* ii)
{
	auto* GizmoWidget =Cast<UGizmoUserWidget> (m_WidgetComp->GetUserWidgetObject());

	if(GizmoWidget)
	{
		GizmoWidget->SetImageAndText(ii,tt);
	}
}
