// All right Reserve 2021 HereticByte


#include "Storage.h"

#include "MyJrpg/MyLib.h"

// Sets default values
AStorage::AStorage()
{
	m_Capsule->InitCapsuleSize(140, 250);

	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshComp"));
	m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComp->CanCharacterStepUpOn = ECB_No;
	m_MeshComp->SetCanEverAffectNavigation(false);
	m_MeshComp->SetupAttachment(RootComponent);
	m_MeshComp->SetMobility(EComponentMobility::Static);
	m_MeshComp->bReceivesDecals = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	FoundW(TEXT("StaticMesh'/Game/08_EnvironmentMesh/BeffioMedievalKingdom/Models/Props/Props_1/SM_Chest.SM_Chest'"));
	static ConstructorHelpers::FClassFinder<UUserWidget>
	FoundW2(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_StorageInfo.WB_StorageInfo_C'"));

	m_MeshComp->SetStaticMesh(FoundW.Object);
	m_ClassInteractWidget = FoundW2.Class;

	m_Capsule->SetMobility(EComponentMobility::Movable);
}//StaticMesh'/Game/08_EnvironmentMesh/BeffioMedievalKingdom/Models/Props/Props_1/SM_Chest.SM_Chest'
//WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_StorageInfo.WB_StorageInfo'
void AStorage::OnInteract()
{
	Super::OnInteract();

	UMyLib::GetCanvas()->OpenStorage();
}

const FBuildDataRow& AStorage::GetBuildData() const
{
	return *m_Data;
}

void AStorage::SetBuildData(const FBuildDataRow& data)
{
	m_Data = &data;
}

void AStorage::SetMat(UMaterialInterface* mat)
{
	
}

void AStorage::ShowBuildWidget(bool b)
{
	
}

void AStorage::ConfirmBuild()
{
	
}

void AStorage::ShowSelect(bool cond)
{
	
}

