#include "BuildData.h"

#include "MyJrpg/Actors/Field/Build/StructureActor.h"

UDataTable* UBuildData::GetBuildTable = nullptr;

UBuildData::UBuildData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Build/BuildDataTable.BuildDataTable'"));
	//
	GetBuildTable = Found.Object;
}

FBuildDataRow::FBuildDataRow(): m_BuildType()
{
	m_ClassActor = AStructureActor::StaticClass(); 
}

UStaticMesh* FBuildDataRow::GetStMesh()
{
	if(m_Mesh.ToSoftObjectPath().IsNull())
	{
		return nullptr;
	}
	return m_Mesh.LoadSynchronous();
}

float FBuildDataRow::GetMeshScale()
{
	return m_fScale;
}
