#include "Op_MSpd.h"

UOp_MSpd::UOp_MSpd()
{
	m_nLevelPerValue = 2;

	m_bIsPercent = true;

	m_OpFormat = TEXT("+{0}%");

	m_OpName = TEXT("추가 이동속도");
}