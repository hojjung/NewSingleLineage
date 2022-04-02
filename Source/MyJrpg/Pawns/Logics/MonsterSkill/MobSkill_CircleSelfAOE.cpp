// Fill out your copyright notice in the Description page of Project Settings.


#include "MobSkill_CircleSelfAOE.h"

#include "MyJrpg/CameraShake/CamShake_LeapAttack.h"
#include "MyJrpg/Pawns/MyPlayerPawn.h"

UMobSkill_CircleSelfAOE::UMobSkill_CircleSelfAOE()
{
	m_Id = TEXT("Skill01");
	m_fRadius = 400;
	m_fDamage = 10;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim(TEXT("AnimMontage'/Game/06_CharacterMesh/MLWarbandPBR/Characters/HogRider/Animations/Attack02Anim_Montage.Attack02Anim_Montage'"));

	m_SkillMotion = Anim.Object;
}

void UMobSkill_CircleSelfAOE::UseSkill()
{
	Super::UseSkill();
	SpawnDecal();
}

void UMobSkill_CircleSelfAOE::OnTriggerSkill()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(),UCamShake_LeapAttack::StaticClass(),m_Owner->GetActorLocation(),m_fRadius,m_fRadius);
	KillDecal();
	TArray<AActor*> Hits;
	if(!TraceSphere(Hits,0,m_fRadius))
	{
		return;
	}

	for(AActor* Hit : Hits)
	{
		AMyPlayerPawn* Mobs = Cast<AMyPlayerPawn>(Hit);

		if(Mobs)
		{
			Mobs->TakeDmg(m_fDamage,m_Owner);
		}
	}
}
