#include "Buff_Base.h"

#include "MyJrpg/DataTables/BuffTable.h"


void UBuff_Base::Init(const FBuffDataRow& data)
{
	m_BuffData = &data;
}

float UBuff_Base::GetDuration() const
{
	return m_BuffData->m_fDuration;
}

void UBuff_Base::OnTick(float delta)
{
	
}

void UBuff_Base::EndBuff()
{
	
}
