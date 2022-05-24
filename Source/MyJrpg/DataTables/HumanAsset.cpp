#include "HumanAsset.h"
#include "MyJrpg/MeshMergeLib.h"
#include "MyJrpg/Pawns/ModularUnitPawn.h"

USkeletalMesh* UHumanAsset::GetSkMesh() const
{
	FSkeletalMeshMergeParams MergeParam;
	MergeParam.Skeleton = m_BodyMesh->GetSkeleton();
	MergeParam.MeshesToMerge.Init(nullptr,(int)EBodyIndex::Len + 1);
	MergeParam.MeshesToMerge[(int)EBodyIndex::Len] = m_BodyMesh;
	MergeParam.MeshesToMerge[(int)EBodyIndex::Head] = m_HeadMesh;
	MergeParam.MeshesToMerge[(int)EBodyIndex::Hat] = m_HatMesh;
	MergeParam.MeshesToMerge[(int)EBodyIndex::Body] = m_TorsoArmorMesh;
	MergeParam.MeshesToMerge[(int)EBodyIndex::Gloves] = m_GlovesMesh;
	MergeParam.MeshesToMerge[(int)EBodyIndex::Legs] = m_LegsMesh;

	USkeletalMesh* SkMeshMerged = UMeshMergeLib::MergeMeshes(MergeParam);

	return SkMeshMerged;
}

UAnimMontage* UHumanAsset::GetStanceAnim(EStanceType t) const
{
	return m_AryAdditionalAnims[(int)t];
}

UAnimMontage* UHumanAsset::GetBaseAtkAnim() const
{
	return GetStanceAnim(m_StanceType);
}
