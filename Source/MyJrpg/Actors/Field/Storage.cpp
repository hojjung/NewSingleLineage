// All right Reserve 2021 HereticByte


#include "Storage.h"

#include "MyJrpg/MyLib.h"

// Sets default values
AStorage::AStorage()
{
	m_Capsule->InitCapsuleSize(120, 250);

	m_WidgetComp = CreateDefaultSubobject<UBuildWidgetCompo>(TEXT("m_WidgetComp"));
	m_WidgetComp->SetupAttachment(RootComponent);
	m_WidgetComp->SetRelativeLocation(FVector(0,0,300));
	
	m_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshComp"));//22
	m_MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComp->CanCharacterStepUpOn = ECB_No;
	m_MeshComp->SetCanEverAffectNavigation(false);
	m_MeshComp->SetupAttachment(m_Capsule);
	m_MeshComp->bReceivesDecals = false;
	m_Capsule->SetRelativeLocation(FVector(100,0,25));
	m_Capsule->SetRelativeRotation(FRotator(0,180,0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	FoundW(TEXT("StaticMesh'/Game/08_EnvironmentMesh/BeffioMedievalKingdom/Models/Props/Props_1/SM_Chest.SM_Chest'"));
	m_MeshComp->SetStaticMesh(FoundW.Object);
	m_MeshComp->SetMobility(EComponentMobility::Movable);
}//StaticMesh'/Game/08_EnvironmentMesh/BeffioMedievalKingdom/Models/Props/Props_1/SM_Chest.SM_Chest'
//WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_StorageInfo.WB_StorageInfo'
void AStorage::OnInteract()
{
	Super::OnInteract();

	UMyLib::GetCanvas()->OpenStorage(m_Inven);
}

void AStorage::BeginPlay()
{
	Super::BeginPlay();
	m_AryMats = m_MeshComp->GetMaterials();
	m_Inven = NewObject<UInventory>(this);
	m_Inven->Init(20);
}

const FBuildDataRow& AStorage::GetBuildData() const
{
	return *m_Data;
}

void AStorage::SetBuildData(const FBuildDataRow& data)
{
	m_Data = &data;
	m_WidgetComp->Init();
}

void AStorage::SetMat(UMaterialInterface* mat)
{
	int Iter = 0;

	while (Iter < m_MeshComp->GetMaterials().Num())
	{
		m_MeshComp->SetMaterial(Iter,mat);
		Iter++;
	}
}

void AStorage::ShowBuildWidget(bool b)
{
	m_WidgetComp->ShowBuildWidget(b);
}

void AStorage::ConfirmBuild()
{
	int Iter = 0;

	while (Iter < m_MeshComp->GetMaterials().Num())
	{
		m_MeshComp->SetMaterial(Iter,m_AryMats[Iter]);
		Iter++;
	}

	SetActorEnableCollision(true);

	m_WidgetComp->SetVisibility(false);
}

void AStorage::ShowSelect(bool b)
{
	m_WidgetComp->ShowSelect(b);
}

bool AStorage::IsEraseable()
{
	return m_Inven->GetUsingSlotCount() <= 0;
}
