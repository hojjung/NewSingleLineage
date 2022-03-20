// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenVector3D.h"

UTweenVector3D* UTweenVector3D::DOVector3D(float DurTime, FVector Start, FVector End, EaseType easeType, bool IsLoop)
{
	UTweenVector3D* result = NewObject<UTweenVector3D>();
	result->Init(DurTime);
	result->StartValue = Start;
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->CurEasyType = easeType;
	return result;
}

void UTweenVector3D::UpdateFunc(float deltaTime)
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
	FVector CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
	OnUpdate.Broadcast(CurValue,this);
}
