// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenCanvasPanelSlot.h"

UTweenCanvasPanelSlot* UTweenCanvasPanelSlot::DOMove(float DurTime, UCanvasPanelSlot* CanvasPanelSlot, FVector2D End,EaseType easeType, bool IsLoop)
{
	if(!CanvasPanelSlot) return nullptr;
	UTweenCanvasPanelSlot* result = NewObject<UTweenCanvasPanelSlot>();
	result->Init(DurTime);
	result->StartValue = CanvasPanelSlot->GetPosition();
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->Slot = CanvasPanelSlot;
	result->TweenCanvasSlotType = ETweenCanvasSlotType::E_CanvasSlotPos;
	result->CurEasyType = easeType;
	return result;
}

UTweenCanvasPanelSlot* UTweenCanvasPanelSlot::DOSize(float DurTime, UCanvasPanelSlot* CanvasPanelSlot, FVector2D End,EaseType easeType, bool IsLoop)
{
	if(!CanvasPanelSlot) return nullptr;
	UTweenCanvasPanelSlot* result = NewObject<UTweenCanvasPanelSlot>();
	result->Init(DurTime);
	result->StartValue = CanvasPanelSlot->GetSize();
	result->EndValue = End;
	result->SetLooping(IsLoop);
	result->Slot = CanvasPanelSlot;
	result->TweenCanvasSlotType = ETweenCanvasSlotType::E_CanvasSlotSize;
	result->CurEasyType = easeType;
	return result;
}

void UTweenCanvasPanelSlot::UpdateFunc(float deltaTime)
{
	Super::UpdateFunc(deltaTime);
	if (!Slot.Get()) return;

	switch (TweenCanvasSlotType)
	{
	case E_CanvasSlotPos:
		{
			FVector2D CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
			Slot->SetPosition(CurValue);
		}
		break;
	case E_CanvasSlotSize:
		{
			FVector2D CurValue = StartValue +  Evaluate() * (EndValue - StartValue);
			Slot->SetSize(CurValue);
		}
		break;
	}
	if (!isInit)
	{
		isInit = true;
		OnFirst.Broadcast(this);
	}
	OnUpdate.Broadcast(this);
	if (Finished)
	{
		OnComplete.Broadcast(this);
	}
}
