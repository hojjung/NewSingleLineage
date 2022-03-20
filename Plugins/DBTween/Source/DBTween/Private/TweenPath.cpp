// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenPath.h"

UTweenPath* UTweenPath::DOPath(TArray<FVector> Path, float DurTime,EaseType easeType, bool IsLoop)
{
	if (Path.Num() == 0) return nullptr;
	
	UTweenPath* result = NewObject<UTweenPath>();
	result->Init(DurTime);
	result->PathValue = Path;
	result->SetLooping(IsLoop);
	result->CalcLength();
	result->EndValue = Path[Path.Num() - 1];
	result->StartValue = Path[0];
	result->CurEasyType = easeType;
	return result;
	
}

void UTweenPath::UpdateFunc(float deltaTime)
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
	float CurLength = FMath::Clamp( Evaluate(),0.f,1.f)  * TotalLength;
	float PreSegmentLength = 0;
	for (int i = 0;i < EachSegmentLength.Num();i++)
	{
		if(CurLength < EachSegmentLength[i])
		{
			CurValue = PathValue[i] + (CurLength - PreSegmentLength)/(EachSegmentLength[i] -PreSegmentLength ) *  (PathValue[i+1] - PathValue[i]);
			break;
		}
		PreSegmentLength +=  EachSegmentLength[i];
		
	}

	OnUpdate.Broadcast(CurValue,this);
}

void UTweenPath::CalcLength()
{
	for (int32 i=1;i<PathValue.Num();i++)
	{
		TotalLength += FVector::Distance(PathValue[i], PathValue[i - 1]);
		EachSegmentLength.Add(TotalLength);
	}
}

