// All right Reserve 2021 HereticByte


#include "PortalActor.h"

#include "Engine/CollisionProfile.h"
#include "MyJrpg/MyLib.h"

// Sets default values
APortalActor::APortalActor()
{
	m_Capsule->InitCapsuleSize(150, 350);
	
	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshComp"));
	m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComp->CanCharacterStepUpOn = ECB_No;
	m_MeshComp->SetCanEverAffectNavigation(false);
	m_MeshComp->SetupAttachment(RootComponent);
	m_MeshComp->SetRelativeScale3D(FVector(1.5f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	FoundW(TEXT("StaticMesh'/Game/08_EnvironmentMesh/BeffioMedievalKingdom/Models/MagicStone/SM_Magic_Stone.SM_Magic_Stone'"));

	m_MeshComp->SetStaticMesh(FoundW.Object);
}//StaticMesh'/Game/08_EnvironmentMesh/BeffioMedievalKingdom/Models/MagicStone/SM_Magic_Stone.SM_Magic_Stone'
//WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_PortalInfo.WB_PortalInfo'
void APortalActor::OnInteract()
{
	UMyLib::GetCanvas()->OpenZone();
}