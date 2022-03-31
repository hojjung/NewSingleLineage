#include "PetPawn.h"
#include "Components/MyMovement.h"
#include "Logics/AI/AI_Logic/AI_LogicBase.h"
#include "Logics/AI/AI_Sensor/Sensor_LogicBase.h"
#include "MyJrpg/MyLib.h"

APetPawn::APetPawn(const FObjectInitializer& objInit):Super(objInit)
{
	m_BodyMesh->SetRelativeLocation(FVector(0,0,100));
	GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APetPawn::SetPetEntity(const FPetRow& pet_row)
{
	m_PFComp->SetMovementComponent(m_Movement);	
	m_PFComp->Initialize();
	m_Movement->SetPathFollowingAgent(m_PFComp);
	m_Movement->MaxSpeed = FGlobalVariable::HERO_DEFAULT_SPEED * 0.9f;
	
	LoadSetSkMeshAnim(pet_row.m_UnitDataAsset);

	m_BodyMesh->SetRelativeScale3D(FVector(pet_row.m_fScale));

	m_PawnName = pet_row.m_ShowingName;
	
	m_PlayerPawn = UMyLib::GetPlayer();

	m_fInitZ = GetSkMesh()->GetRelativeLocation().Z;

	m_fFloatingZ = 10;

	m_fMaxZ = m_fInitZ + m_fFloatingZ;

	m_fMinZ = m_fInitZ - m_fFloatingZ;

	m_bIsMax = true;

	m_fTimer = 0;

	m_fFloatingTime = 0.6f;
}

void APetPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AnimateFly(DeltaSeconds);

	MoveToActor(m_PlayerPawn,100);
}

void APetPawn::AnimateFly(float DeltaSeconds)
{
	float LerpZ = FMath::Lerp<float>(m_bIsMax ? m_fMinZ : m_fMaxZ,m_bIsMax ? m_fMaxZ : m_fMinZ,m_fTimer / m_fFloatingTime);

	m_BodyMesh->SetRelativeLocation(FVector(0,0,LerpZ));
		
	m_fTimer += DeltaSeconds;

	if(m_fTimer >= m_fFloatingTime)
	{
		m_fTimer = 0;

		m_bIsMax = !m_bIsMax;
	}
}
