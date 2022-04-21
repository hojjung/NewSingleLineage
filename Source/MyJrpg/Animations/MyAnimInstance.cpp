#include "MyAnimInstance.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void FMyAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	Super::InitializeObjects(InAnimInstance);
    
	m_MyAnim = Cast<UMyAnimInstance>(InAnimInstance);
}

void FMyAnimInstanceProxy::Update(float DeltaSeconds)
{
	m_MyAnim->UpdateMoveFlag();
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Owner=Cast<ACombatUnitPawn>( TryGetPawnOwner());
}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_Owner=Cast<ACombatUnitPawn>( TryGetPawnOwner());
}

void UMyAnimInstance::UpdateMoveFlag()
{
//#if WITH_EDITOR
	if(!m_Owner)
	{
		return;
	}
//#endif
	m_bIsMoving=m_Owner->IsMoving();
	m_bIsRange=m_Owner->IsRange();
	m_bIsSneaking=m_Owner->IsSneak();
}
