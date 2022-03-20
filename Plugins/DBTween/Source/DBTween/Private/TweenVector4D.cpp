// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenVector4D.h"

UTweenVector4D* UTweenVector4D::DOVector4D(FVector4 Start, FVector4 End,float DurTime, EaseType easeType, bool IsLoop)
{
	UTweenVector4D* result = NewObject<UTweenVector4D>();
	result->Init(DurTime);
	result->StartValue = Start;
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->CurEasyType = easeType;
	return result;
}


void UTweenVector4D::UpdateFunc(float deltaTime)
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
	FVector4 CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
	OnUpdate.Broadcast(CurValue,this);
}
