#include "AnimNotify_Trigger.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyJrpg/Pawns/BaseUnitPawn.h"
#include "MyJrpg/Pawns/CombatUnitPawn.h"

void UAnimNotify_Trigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(!MeshComp->GetOwner())
	{
		return;
	}

	ACombatUnitPawn* Pawn =  Cast<ACombatUnitPawn>( MeshComp->GetOwner());

	if(!Pawn)
	{
		return;
	}

	Pawn->OnNotifyTrigger(m_ID);
}
