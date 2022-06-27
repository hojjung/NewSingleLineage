#include "MinimapCam.h"

#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


AMinimapCam::AMinimapCam()//TextureRenderTarget2D'/Game/03_VisualEffect/T_Minimap.T_Minimap'
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_Capture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture2D");
	m_Capture->SetupAttachment(RootComponent);
	m_Capture->SetRelativeLocation(FVector(-600,0,3000));
	m_Capture->SetRelativeRotation(FRotator(-90,0,0));
	m_Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	m_Capture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	m_Capture->SetTickableWhenPaused(true);
	m_Capture->ProjectionType = ECameraProjectionMode::Orthographic;
	m_Capture->OrthoWidth = 2300;
	m_Capture->PostProcessBlendWeight = 0.f;
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> Found(TEXT("TextureRenderTarget2D'/Game/03_VisualEffect/T_Minimap.T_Minimap'"));
	m_Capture->TextureTarget = Found.Object;
}

void AMinimapCam::BeginPlay()
{
	Super::BeginPlay();

	UCameraComponent* Cam = UMyLib::GetPlayer()->GetCameraComp();
	
	AttachToComponent(Cam, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,true));
}

void AMinimapCam::AddTrackIcon(IFocusable* icon)
{
	m_Capture->ShowOnlyComponents.Add(icon->GetIconMeshComp()->GetMeshComp());
}