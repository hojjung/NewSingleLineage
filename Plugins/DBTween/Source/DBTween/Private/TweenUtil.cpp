// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenUtil.h"

UTweenActor* UTweenUtil::DOLocalMoveSync(float DurTime /*= 1.f*/, AActor* Actor /*= nullptr*/, FVector End /*= FVector(100, 100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenActor::DOLocalMove(DurTime, Actor, End, easeType, IsLoop);
}

UTweenActor* UTweenUtil::DOLocalRotationSync(float DurTime /*= 1.f*/, AActor* Actor /*= nullptr*/, FRotator End /*= FRotator(0, 90, 0)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenActor::DOLocalRotation(DurTime, Actor, End, easeType, IsLoop);
}

UTweenActor* UTweenUtil::DOLocalScaleSync(float DurTime /*= 1.f*/, AActor* Actor /*= nullptr*/, FVector End /*= FVector(2, 2, 2)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenActor::DOLocalScale(DurTime, Actor, End, easeType, IsLoop);
}

UTweenActor* UTweenUtil::DOMoveSync(float DurTime /*= 1.f*/, AActor* Actor /*= nullptr*/, FVector End /*= FVector(100, 100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenActor::DOMove(DurTime, Actor, End, easeType, IsLoop);
}

UTweenActor* UTweenUtil::DORotationSync(float DurTime /*= 1.f*/, AActor* Actor /*= nullptr*/, FRotator End /*= FRotator(0, 90, 0)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenActor::DORotation(DurTime, Actor, End, easeType, IsLoop);
}

UTweenActor* UTweenUtil::DOScaleSync(float DurTime /*= 1.f*/, AActor* Actor /*= nullptr*/, FVector End /*= FVector(2, 2, 2)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenActor::DOScale(DurTime, Actor, End, easeType, IsLoop);
}

UTweenColor* UTweenUtil::DOColorSync(float DurTime /*= 1*/, FLinearColor Start /*= FLinearColor(0, 0, 0, 1)*/, FLinearColor End /*= FLinearColor(1, 1, 1, 1)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenColor::DOColor(DurTime, Start, End, easeType, IsLoop);
}

UTweenFloat* UTweenUtil::DOFloatSync(float DurTime /*= 1*/, float Start /*= 0*/, float End /*= 1*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenFloat::DOFloat(DurTime, Start, End, easeType, IsLoop);
}


UTweenPath* UTweenUtil::DOPathSync(TArray<FVector> Path, float DurTime /*= 1*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenPath::DOPath(Path, DurTime, easeType, IsLoop);
}

UTweenPath* UTweenUtil::DOPathJumpSync(FVector start, FVector end, float height, float DurTime, EaseType easeType)
{
	FVector Result;
	Result.X = start.X + ((end.X - start.X) / 2.f);
	Result.Y = start.Y + ((end.Y - start.Y) / 2.f);
	Result.Z = start.Z + ((end.Z - start.Z) / 2.f);
	Result.Z += height;
	
	TArray<FVector> Path;

	Path.Add(start);
	Path.Add(Result);
	Path.Add(end);

	return DOPathSync(Path, DurTime, easeType, false);
}

UTweenVector2D* UTweenUtil::DOVector2DSync(float DurTime /*= 1*/, FVector2D Start /*= FVector2D(0, 0)*/, FVector2D End /*= FVector2D(100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenVector2D::DOVector2D(DurTime, Start, End, easeType, IsLoop);
}

UTweenVector3D* UTweenUtil::DOVector3DSync(float DurTime /*= 1.f*/, FVector Start /*= FVector(0, 0, 0)*/, FVector End /*= FVector(100, 100, 100)*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenVector3D::DOVector3D(DurTime, Start, End, easeType, IsLoop);
}

UTweenVector4D* UTweenUtil::DOVector4DSync(FVector4 Start, FVector4 End, float DurTime /*= 1.f*/, EaseType easeType /*= EaseType::Linear*/, bool IsLoop /*= false*/)
{
	return UTweenVector4D::DOVector4D( Start, End, DurTime, easeType, IsLoop);
}
