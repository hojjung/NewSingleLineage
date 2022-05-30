// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffTable.h"

UDataTable* UBuffTable::GetBuffTable = nullptr;

UBuffTable::UBuffTable()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Found(TEXT("DataTable'/Game/00_DataTables/Item/BuffTable.BuffTable'"));
	//
	GetBuffTable = Found.Object;
}
