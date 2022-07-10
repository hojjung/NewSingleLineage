// Fill out your copyright notice in the Description page of Project Settings.


#include "CashProductTable.h"

UDataTable* UCashProductTable::GetCashTable = nullptr;

UCashProductTable::UCashProductTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Cash/CashData.CashData'"));
	//
	GetCashTable = Found.Object;	
}
