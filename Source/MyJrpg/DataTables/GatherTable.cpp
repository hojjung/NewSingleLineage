#include "GatherTable.h"

UDataTable* UGatherTable::GetGatherTable = nullptr;

UGatherTable::UGatherTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Gather/GatherTable.GatherTable'"));
	//
	GetGatherTable = Found.Object;

}

UStaticMesh* FGatherDataRow::GetStMesh()
{
	if(m_GatherAsset.ToSoftObjectPath().IsNull())
	{
		return nullptr;
	}
	
	return m_GatherAsset.LoadSynchronous()->m_TopMesh;
}

float FGatherDataRow::GetMeshScale()
{
	return m_fTopMeshScale;
}
