// Fill out your copyright notice in the Description page of Project Settings.


#include "KarmaDown.h"

#include "MyJrpg/Managers/MyGameInstance.h"

void UKarmaDown::RecieveEventTriggered(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	Super::RecieveEventTriggered(ConsideringPlayer, NPCActor);

	UMyGameInstance::Get->m_TeamKarma->DecreaseKarma(TEXT("Village"), 100);
}
