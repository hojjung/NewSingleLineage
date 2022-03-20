// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenColor.h"

UTweenColor* UTweenColor::DOColor(float DurTime, FLinearColor Start, FLinearColor End, EaseType easeType, bool IsLoop)
{
	UTweenColor* result = NewObject<UTweenColor>();
	result->Init(DurTime);
	result->StartValue = Start;
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->CurEasyType = easeType;
	return result;
}

void UTweenColor::UpdateFunc(float deltaTime)
{
	if (Paused)return;
	Super::UpdateFunc(deltaTime);
	if (Finished)
	{
		OnUpdate.Broadcast(EndValue, this);
		OnComplete.Broadcast(EndValue, this);
		return;
	}
	if (!isInit)
	{
		isInit = true;
		OnFirst.Broadcast(StartValue,this);
	}
	FLinearColor CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
	OnUpdate.Broadcast(CurValue,this);
}
