// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenUtil2.h"

UTweenCanvasPanelSlot* UTweenUtil2::DOMoveSync(float DurTime /*= 1*/, UCanvasPanelSlot* CanvasPanelSlot /*= nullptr*/, FVector2D End /*= FVector2D(100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenCanvasPanelSlot::DOMove(DurTime, CanvasPanelSlot, End, easeType, IsLoop);
}

UTweenCanvasPanelSlot* UTweenUtil2::DOSizeSync(float DurTime /*= 1*/, UCanvasPanelSlot* CanvasPanelSlot /*= nullptr*/, FVector2D End /*= FVector2D(100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenCanvasPanelSlot::DOSize(DurTime, CanvasPanelSlot, End, easeType, IsLoop);
}

UTweenWidget* UTweenUtil2::DOLocalMoveSync(float DurTime /*= 1*/, UWidget* UI /*= nullptr*/, FVector2D End /*= FVector2D(100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenWidget::DOLocalMove(DurTime, UI, End, easeType, IsLoop);
}

UTweenWidget* UTweenUtil2::DOLocalAngleSync(float DurTime /*= 1*/, UWidget* UI /*= nullptr*/, float End /*= 90*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenWidget::DOLocalAngle(DurTime, UI, End, easeType, IsLoop);
}

UTweenWidget* UTweenUtil2::DOLocalScaleSync(float DurTime /*= 1*/, UWidget* UI /*= nullptr*/, FVector2D End /*= FVector2D(2, 2)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenWidget::DOLocalScale(DurTime, UI, End, easeType, IsLoop);
}

UTweenWidget* UTweenUtil2::DOFadeSync(float DurTime /*= 1*/, UWidget* UI /*= nullptr*/, float End /*= 1*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenWidget::DOFade(DurTime, UI, End, easeType, IsLoop);
}
