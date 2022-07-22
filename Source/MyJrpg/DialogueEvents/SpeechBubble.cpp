// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechBubble.h"

#include "MyJrpg/Pawns/MonsterPawn.h"

void USpeechBubble::RecieveEventTriggered(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	Super::RecieveEventTriggered(ConsideringPlayer, NPCActor);
	
	AMonsterPawn* Mob = Cast<AMonsterPawn>(NPCActor);

	Mob->Speech(TEXT("살려줘 꼬꼬댁!!!!!!"));
}
