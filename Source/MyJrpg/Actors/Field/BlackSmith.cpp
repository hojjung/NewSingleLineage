#include "BlackSmith.h"
#include "MyJrpg/MyLib.h"

ABlackSmith::ABlackSmith()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		FoundW(TEXT("SkeletalMesh'/Game/06_CharacterMesh/ModularNPC/SK_ma_head_02_SK.SK_ma_head_02_SK'"));
	m_MeshComp->SetSkeletalMesh(FoundW.Object);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		FoundAnim(TEXT("AnimSequence'/Game/09_SharedAnimations/NpcAnims/NPC_Anim__Smith_Working.NPC_Anim__Smith_Working'"));
	m_IdleAnim = FoundAnim.Object;
}
//SkeletalMesh'/Game/06_CharacterMesh/ModularNPC/SK_ma_head_02_SK.SK_ma_head_02_SK'
//AnimSequence'/Game/09_SharedAnimations/NpcAnims/NPC_Anim__Smith_Working.NPC_Anim__Smith_Working'
void ABlackSmith::OnInteract()
{
	UMyLib::GetCanvas()->OpenShop(TEXT("BlackSmith"));
}
