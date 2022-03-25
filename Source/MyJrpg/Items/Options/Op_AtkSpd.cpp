// Fill out your copyright notice in the Description page of Project Settings.


#include "Op_AtkSpd.h"

UOp_AtkSpd::UOp_AtkSpd()
{
	m_nLevelPerValue = 1;

	m_bIsPercent = true;

	m_OpFormat = TEXT("+{0}%");

	m_OpName = TEXT("추가 공격속도");
}
