#include "MinimapCompParent.h"

UMinimapCompParent::UMinimapCompParent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_RelativeSocketRotation = FQuat::Identity;
}

FTransform UMinimapCompParent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
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

void UMinimapCompParent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UMinimapCompParent::SetRotationOffset(FRotator rot)
{
	m_RotOffset = rot;
}

