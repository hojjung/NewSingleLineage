// All right Reserve 2021 HereticByte


#include "ItemData.h"

#include "MyJrpg/MyLib.h"
#include "UObject/ConstructorHelpers.h"

UDataTable* UItemData::GetItemTable = nullptr;

UDataTable* UItemData::GetColorTable = nullptr;

FStatGroup FStatGroup::operator+(const FStatGroup& stat_group) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	+= stat_group.m_nAvoid;
	StatNew.m_nAccu		+= stat_group.m_nAccu;
	StatNew.m_MaxHp		+= stat_group.m_MaxHp;
	StatNew.m_Hp		+= stat_group.m_Hp;
	StatNew.m_Dmg		+= stat_group.m_Dmg;
	StatNew.m_AtkSpeed	+= stat_group.m_AtkSpeed;
	StatNew.m_MoveSpeed	+= stat_group.m_MoveSpeed;
	StatNew.m_CriPer	+= stat_group.m_CriPer;
	StatNew.m_CriDmg	+= stat_group.m_CriDmg;
	StatNew.m_DmgReduce += stat_group.m_DmgReduce;
	
	return StatNew;
}

FStatGroup FStatGroup::operator*(const FStatGroup& stat_group) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	*= stat_group.m_nAvoid;
	StatNew.m_nAccu		*= stat_group.m_nAccu;
	StatNew.m_MaxHp		*= stat_group.m_MaxHp;
	StatNew.m_Hp		*= stat_group.m_Hp;
	StatNew.m_Dmg		*= stat_group.m_Dmg;
	StatNew.m_AtkSpeed	*= stat_group.m_AtkSpeed;
	StatNew.m_MoveSpeed	*= stat_group.m_MoveSpeed;
	StatNew.m_CriPer	*= stat_group.m_CriPer;
	StatNew.m_CriDmg	*= stat_group.m_CriDmg;
	StatNew.m_DmgReduce *= stat_group.m_DmgReduce;
	
	return StatNew;
}

FStatGroup FStatGroup::operator*(float m) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	*= m;
	StatNew.m_nAccu		*= m;
	StatNew.m_MaxHp		*= m;
	StatNew.m_Hp		*= m;
	StatNew.m_Dmg		*= m;
	StatNew.m_AtkSpeed	*= m;
	StatNew.m_MoveSpeed	*= m;
	StatNew.m_CriPer	*= m;
	StatNew.m_CriDmg	*= m;
	StatNew.m_DmgReduce *= m;
	
	return StatNew;
}

FStatGroup FStatGroup::operator*(int m) const
{
	FStatGroup StatNew = *this;
	StatNew.m_nAvoid	*= m;
	StatNew.m_nAccu		*= m;
	StatNew.m_MaxHp		*= m;
	StatNew.m_Hp		*= m;
	StatNew.m_Dmg		*= m;
	StatNew.m_AtkSpeed	*= m;
	StatNew.m_MoveSpeed	*= m;
	StatNew.m_CriPer	*= m;
	StatNew.m_CriDmg	*= m;
	StatNew.m_DmgReduce *= m;
	
	return StatNew; 
}

FStatGroup& FStatGroup::operator+=(const FStatGroup& stat_group)
{
	m_nAvoid	+= stat_group.m_nAvoid;
	m_nAccu		+= stat_group.m_nAccu;
	m_MaxHp		+= stat_group.m_MaxHp;
	m_Hp		+= stat_group.m_Hp;
	m_Dmg		+= stat_group.m_Dmg;
	m_AtkSpeed	+= stat_group.m_AtkSpeed;
	m_MoveSpeed	+= stat_group.m_MoveSpeed;
	m_CriPer	+= stat_group.m_CriPer;
	m_CriDmg	+= stat_group.m_CriDmg;
	m_DmgReduce += stat_group.m_DmgReduce;
	
	return *this;
}

FStatGroup& FStatGroup::operator-=(const FStatGroup& stat_group)
{
	m_nAvoid	-= stat_group.m_nAvoid;
	m_nAccu		-= stat_group.m_nAccu;
	m_MaxHp		-= stat_group.m_MaxHp;
	m_Hp		-= stat_group.m_Hp;
	m_Dmg		-= stat_group.m_Dmg;
	m_AtkSpeed	-= stat_group.m_AtkSpeed;
	m_MoveSpeed	-= stat_group.m_MoveSpeed;
	m_CriPer	-= stat_group.m_CriPer;
	m_CriDmg	-= stat_group.m_CriDmg;
	m_DmgReduce -= stat_group.m_DmgReduce;
	
	return *this;
}

UItemData::UItemData()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Item/ItemTable.ItemTable'"));
	//
	GetItemTable = Found.Object;
	//
	static ConstructorHelpers::FObjectFinder<UDataTable> Found2(TEXT("DataTable'/Game/00_DataTables/ColorTable.ColorTable'"));
	//
	GetColorTable = Found2.Object;
}

TSoftObjectPtr<UTexture2D> FCraftItemCost::GetItemIcon() const
{
	if(m_ItemDataRowHandle.IsNull())
	{
		return nullptr;
	}
		
	return m_ItemDataRowHandle.GetRow<FItemDataRow>("")->m_ItemIcon;
}