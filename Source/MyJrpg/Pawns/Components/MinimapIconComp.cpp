#include "MinimapIconComp.h"

#include "MyJrpg/Managers/MyGameInstance.h"

UMinimapIconComp::UMinimapIconComp(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	m_RelativeSocketRotation = FQuat::Identity;
	m_bUseRot = true;
	//
	m_IconMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("m_IconMeshComp");
	m_IconMeshComp->SetupAttachment(GetAttachmentRoot());
	m_IconMeshComp->SetRelativeLocation(FVector(0,0,1800.f));
	m_IconMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_IconMeshComp->SetCanEverAffectNavigation(false);
	m_IconMeshComp->bReceivesDecals = false;
	m_IconMeshComp->SetOwnerNoSee(true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundIconMesh(
		TEXT("StaticMesh'/Game/03_VisualEffect/SM_Icon.SM_Icon'"));
	m_IconMeshComp->SetStaticMesh(FoundIconMesh.Object);
}

FTransform UMinimapIconComp::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	if(m_bUseRot)
	{
		return Super::GetSocketTransform(InSocketName, TransformSpace);
	}
	FTransform RelativeTransform(m_RelativeSocketRotation, m_RelativeSocketLocation);

	switch (TransformSpace)
	{
	case RTS_World:
		{
			return RelativeTransform * GetComponentTransform();
		}
	case RTS_Actor:
		{
			if (const AActor* Actor = GetOwner())
			{
				FTransform SocketTransform = RelativeTransform * GetComponentTransform();
				return SocketTransform.GetRelativeTransform(Actor->GetTransform());
			}
			break;
		}
	case RTS_Component:
		{
			return RelativeTransform;
		}
	}
	return RelativeTransform;
}

void UMinimapIconComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator DesiredRot = GetRelativeRotation() + m_RotOffset;

	FVector ArmOrigin = GetComponentLocation();
	
	FVector DesiredLoc = ArmOrigin;
	
	FTransform WorldCamTM(DesiredRot, DesiredLoc);
	
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());
	
	m_RelativeSocketLocation = RelCamTM.GetLocation();
	
	m_RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}

void UMinimapIconComp::SetRotationOffset(FRotator rot)
{
	m_bUseRot = false;

	m_RotOffset = rot;
}

UStaticMeshComponent* UMinimapIconComp::GetMeshComp()
{
	return m_IconMeshComp;
}

void UMinimapIconComp::SetIcon(UTexture2D* wantIcon)
{
	auto* MatDynamic = UMaterialInstanceDynamic::Create(m_IconMeshComp->GetMaterial(0), this);
	
	MatDynamic->SetTextureParameterValue(TEXT("Icon"),wantIcon);
	
	m_IconMeshComp->SetMaterial(0, MatDynamic);
}

void UMinimapIconComp::SetLayerHeight(float h)
{
	m_IconMeshComp->SetRelativeLocation(FVector(0,0,1800.f - h));
}
