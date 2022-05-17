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
	
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_PlOwner=Cast<AMyPlayerPawn>( TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeInitializeAnimation() 
{
	Super::NativeInitializeAnimation();
	m_PlOwner=Cast<AMyPlayerPawn>( TryGetPawnOwner());
}

void UPlayerAnimInstance::UpdateMoveFlag()
{
	//#if WITH_EDITOR
	if(!m_PlOwner)
	{
		return;
	}
	//#endif
	m_bIsMoving=m_PlOwner->IsMoving();
	m_bIsRange=m_PlOwner->IsRange();
	m_bIsSneaking=m_PlOwner->IsSneak();
	m_Stance=m_PlOwner->GetStance();
}

bool UPlayerAnimInstance::IsStance(EStanceType t) const
{
	return m_Stance == t;
}

bool UPlayerAnimInstance::IsNotStance(EStanceType t) const
{
	return m_Stance != t;
}
