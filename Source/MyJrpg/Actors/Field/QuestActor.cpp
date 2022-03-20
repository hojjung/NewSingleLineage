// All right Reserve 2021 HereticByte


#include "QuestActor.h"

#include "MyJrpg/MyLib.h"

// Sets default values
AQuestActor::AQuestActor()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>
	FoundW2(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_QuestInfo.WB_QuestInfo_C'"));

	m_ClassInteractWidget = FoundW2.Class;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		FoundW(TEXT("SkeletalMesh'/Game/06_CharacterMesh/ModularNPC/SK_ma_head_02_SK2.SK_ma_head_02_SK2'"));
	m_MeshComp->SetSkeletalMesh(FoundW.Object);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		FoundAnim(TEXT("AnimSequence'/Game/09_SharedAnimations/NpcAnims/NPC_Anim_Call_Someone_1.NPC_Anim_Call_Someone_1'"));
	m_IdleAnim = FoundAnim.Object;
}//WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_QuestInfo.WB_QuestInfo'
//SkeletalMesh'/Game/06_CharacterMesh/ModularNPC/SK_ma_head_02_SK2.SK_ma_head_02_SK2'
//AnimSequence'/Game/09_SharedAnimations/NpcAnims/NPC_Anim_Call_Someone_1.NPC_Anim_Call_Someone_1'
void AQuestActor::OnInteract()
{
	UMyLib::GetCanvas()->StartDialogue(TEXT("QuestActor"));
}