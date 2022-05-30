// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff_Test.h"

#include "MyJrpg/MyJrpg.h"

void UBuff_Test::OnTick(float delta)
{
	Super::OnTick(delta);

	PRINTF("Test Buff OnTick");
}

void UBuff_Test::Init(const FBuffDataRow& data)
{
	Super::Init(data);
	PRINTF("Test Buff Init");
}

void UBuff_Test::EndBuff()
{
	Super::EndBuff();
	PRINTF("Test Buff EndBuff");
}
