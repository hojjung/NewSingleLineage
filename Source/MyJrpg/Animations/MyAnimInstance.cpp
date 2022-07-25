#include "MyAnimInstance.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

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

	bUseMultiThreadedAnimationUpdate = true;

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
	m_bIsSneaking=m_Owner->IsSneak();
	m_Stance=m_Owner->GetStance();
	m_bIsLooting=m_Owner->IsLooting();
}

bool UMyAnimInstance::IsStance(EStanceType t) const
{
	return m_Stance == t;
}

bool UMyAnimInstance::IsNotStance(EStanceType t) const
{
	return m_Stance != t;
}

int UMyAnimInstance::GetStanceIndex() const
{
	if(m_bIsLooting)
	{
		return (int)EStanceType::Length;
	}
	return (int)m_Stance;
}
