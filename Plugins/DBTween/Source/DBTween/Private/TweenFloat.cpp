// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenFloat.h"

UTweenFloat* UTweenFloat::DOFloat(float DurTime, float Start, float End, EaseType easeType, bool IsLoop)
{
	UTweenFloat* result = NewObject<UTweenFloat>();
	result->Init(DurTime);
	result->StartValue = Start;
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->CurEasyType = easeType;
	return result;
}

void UTweenFloat::UpdateFunc(float deltaTime)
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
	float CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
	OnUpdate.Broadcast(CurValue,this);
}
