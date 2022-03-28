// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetCollecItemEle.h"

void UWidgetCollecItemEle::Init(const FName& collecID,bool is_equip, const FItemDataHandle& item, int lv)
{
	//Enable Disable = 해당 아이템을 가지고 있는지 여부에따라서

	//해당 아이템이 장착아이템이고 강화레벨이 낮을때 = 빨간색 강화레벨

	//아이템도, 강화레벨도 일치할때 = 엔에이블 활성화

	//콜렉션 등록 완료시 = 체크박스 좌상단에 생김
}

void UWidgetCollecItemEle::Update()
{
	
}
