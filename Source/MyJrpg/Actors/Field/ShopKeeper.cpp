// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopKeeper.h"

#include "MyJrpg/MyLib.h"

AShopKeeper::AShopKeeper()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>
	FoundW2(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_ShopInfo.WB_ShopInfo_C'"));

	m_ClassInteractWidget = FoundW2.Class;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		FoundW(TEXT("SkeletalMesh'/Game/06_CharacterMesh/ModularNPC/SK_fe_head_01_SK.SK_fe_head_01_SK'"));
	
	static ConstructorHelpers::FObjectFinder<UAnimationAsset>
		FoundAnim(TEXT("AnimSequence'/Game/09_SharedAnimations/NpcAnims/NPC_Anim_Showing_Goods_2.NPC_Anim_Showing_Goods_2'"));

	m_IdleAnim = FoundAnim.Object;
	m_MeshComp->SetSkeletalMesh(FoundW.Object);
}//WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/Village/WB_ShopInfo.WB_ShopInfo'
//SkeletalMesh'/Game/06_CharacterMesh/ModularNPC/SK_fe_head_01_SK.SK_fe_head_01_SK'
//AnimSequence'/Game/09_SharedAnimations/NpcAnims/NPC_Anim_Showing_Goods_2.NPC_Anim_Showing_Goods_2'
void AShopKeeper::OnInteract()
{
	UMyLib::GetCanvas()->OpenShop(TEXT("Shop"));
}
