// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenVector2D.h"

UTweenVector2D* UTweenVector2D::DOVector2D(float DurTime, FVector2D Start, FVector2D End, EaseType easeType,bool IsLoop)
{
	UTweenVector2D* result = NewObject<UTweenVector2D>();
	result->Init(DurTime);
	result->StartValue = Start;
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->CurEasyType = easeType;
	return result;
}

void UTweenVector2D::UpdateFunc(float deltaTime)
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
		OnFirst.Broadcast(StartValue, this);
	}
	FVector2D CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
	OnUpdate.Broadcast(CurValue,this);
}
