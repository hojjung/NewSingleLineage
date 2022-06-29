#include "ItemActor.h"
#include "Components/BillboardComponent.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/EquipManager.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/WidgetComponents/WidgetSpeechBubble.h"
#include "MyJrpg/Widgets/WidgetComponents/WidgetSpeechBubbleComp.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	m_BillboardIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("m_BillboardIcon"));
	m_BillboardIcon->SetHiddenInGame(false);
	m_BillboardIcon->SetupAttachment(RootComponent);//-70
	m_BillboardIcon->SetRelativeScale3D(FVector(2));
	m_BillboardIcon->SetRelativeLocation(FVector(0,0,-56));
	m_BillboardIcon->bReceivesDecals = false;
	//
	m_MeshItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_MeshTree"));
	m_MeshItem->CanCharacterStepUpOn = ECB_No;
	m_MeshItem->SetCanEverAffectNavigation(false);
	m_MeshItem->SetupAttachment(RootComponent);
	m_MeshItem->bReceivesDecals = false;
	m_MeshItem->SetRelativeLocation(FVector(0,0,-88));
	m_MeshItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//AnimMontage'/Game/09_SharedAnimations/Loot_Anim_Set/Loot_Corpse_GrabItem_Montage.Loot_Corpse_GrabItem_Montage'
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	FoundAnim(TEXT("AnimMontage'/Game/09_SharedAnimations/Loot_Anim_Set/Loot_Corpse_GrabItem_Montage.Loot_Corpse_GrabItem_Montage'"));
	m_Anim = FoundAnim.Object;
	//
	m_IconMeshComp = CreateDefaultSubobject<UMinimapIconComp>("m_IconMeshComp");
	m_IconMeshComp->SetupAttachment(RootComponent);
	m_IconMeshComp->SetRelativeScale3D(FVector(0.7f));
}

void AItemActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_MeshItem->SetStaticMesh(nullptr);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	UMyGameInstance::Get->m_ZoneInst->AddTrackIcon(this);
}

void AItemActor::Init(FName itemID, int countOrLevel)
{
	m_NavProp.AgentHeight = 88;
	m_NavProp.AgentRadius = 34;
	
	m_ItemSpec.m_ID = itemID;

	m_ItemSpec.m_nLvStack = countOrLevel;
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemSpec.m_ID);

	if(ItemData.m_ItemDropMesh.ToSoftObjectPath().IsValid())
	{
		m_BillboardIcon->SetHiddenInGame(true);
		
		m_MeshItem->SetStaticMesh(ItemData.m_ItemDropMesh.LoadSynchronous());
		m_MeshItem->SetRelativeScale3D(FVector(ItemData.m_fItemDropMeshScale));
	}
	else
	{
		m_MeshItem->SetHiddenInGame(true);
		
		UTexture2D* t = UMyAssetManager::Get()->LoadTexture(ItemData.m_Icon);

		m_BillboardIcon->SetSprite(t);
	}

	m_IconMeshComp->SetRotationOffset(FRotator(0,45,0));

	m_IconMeshComp->SetIcon(ItemData.m_Icon.LoadSynchronous());
}

void AItemActor::OnInteract()
{
	UMyLib::GetPlayer()->SetInteracting(true);
	
	UMyLib::GetPlayer()->RequestInteract(this,FVoidVoid::CreateUObject(this,&AItemActor::OnArrived),150);
}

void AItemActor::OnArrived()
{
	UMyLib::GetPlayer()->PlayAnimMontage(m_Anim);
}

void AItemActor::OnObtain()//주변에서 누가 보고있으면
{
	UMyLib::GetPlayer()->SetInteracting(false);
	UMyLib::GetPlayer()->SetFocusedTarget(nullptr);

	if(UMyLib::GetEquip()->AddItem(m_ItemSpec))
	{
		UMyGameInstance::Get->m_PlayerStatManager->AddExp(3);
		UMyGameInstance::Get->m_ZoneInst->RemoveFocusActor(this);
		Destroy();
	}
}

bool AItemActor::HasOwnerTeamID() const
{
	return !m_OwnerID.IsNone();
}

FVector AItemActor::GetNavAgentLocation() const
{
	return GetActorLocation() - FVector(0.f, 0.f, 88);
}

const FNavAgentProperties& AItemActor::GetNavAgentPropertiesRef() const
{
	return m_NavProp;
}

void AItemActor::GetMoveGoalReachTest(const AActor* MovingActor, const FVector& MoveOffset, FVector& GoalOffset,float& GoalRadius, float& GoalHalfHeight) const
{
	GoalOffset = FVector::ZeroVector;
	
	GoalRadius = GetNavAgentPropertiesRef().AgentRadius;

	GoalHalfHeight = GetNavAgentPropertiesRef().AgentHeight;
}

const FItemSpec& AItemActor::GetItemSpec() const
{
	return m_ItemSpec;
}

FText AItemActor::GetTextInteract()
{
	return NSLOCTEXT("AItemActor","Loot","줍기");
}

bool AItemActor::IsInteractable()
{
	return true;
}

UMinimapIconComp* AItemActor::GetIconMeshComp()
{
	return m_IconMeshComp;
}
