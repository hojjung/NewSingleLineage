// Copyright 2021-2022, DearBing. All Rights Reserved.


#include "TweenActor.h"

#include "GameFramework/Actor.h"

UTweenActor* UTweenActor::DOLocalMove(float DurTime, AActor* Actor, FVector End, EaseType easeType, bool IsLoop)
{
	if(!Actor) return nullptr;
	UTweenActor* result = NewObject<UTweenActor>();
	result->Init(DurTime);
	result->StartLocValue = Actor->GetRootComponent()->GetRelativeLocation();
	result->EndLocValue = End;
	result->SetLooping(IsLoop);
	result->Actor = Actor;
	result->TweenActorType = ETweenActorType::E_ActorRelativeLocation;
	result->CurEasyType = easeType;
	return result;
}

UTweenActor* UTweenActor::DOLocalRotation(float DurTime, AActor* Actor, FRotator End, EaseType easeType, bool IsLoop)
{
	if(!Actor) return nullptr;
	UTweenActor* result = NewObject<UTweenActor>();
	result->Init(DurTime);
	result->StartRotValue = Actor->GetRootComponent()->GetRelativeRotation().Quaternion();
	result->EndRotValue = End.Quaternion();
	result->SetLooping(IsLoop);
	result->Actor = Actor;
	result->TweenActorType = ETweenActorType::E_ActorRelativeRotation;
	result->CurEasyType = easeType;
	return result;
}

UTweenActor* UTweenActor::DOLocalScale(float DurTime, AActor* Actor, FVector End, EaseType easeType, bool IsLoop)
{
	if(!Actor) return nullptr;
	UTweenActor* result = NewObject<UTweenActor>();
	result->Init(DurTime);
	result->StartScaleValue = Actor->GetActorRelativeScale3D();
	result->EndScaleValue = End;
	result->SetLooping(IsLoop);
	result->Actor = Actor;
	result->TweenActorType = ETweenActorType::E_ActorRelativeScale;
	result->CurEasyType = easeType;
	return result;
}

UTweenActor* UTweenActor::DOMove(float DurTime, AActor* Actor, FVector End, EaseType easeType, bool IsLoop)
{
	if(!Actor) return nullptr;
	UTweenActor* result = NewObject<UTweenActor>();
	result->Init(DurTime);
	result->StartLocValue = Actor->GetActorLocation();
	result->EndLocValue = End;
	result->SetLooping(IsLoop);
	result->Actor = Actor;
	result->TweenActorType = ETweenActorType::E_ActorLocation;
	result->CurEasyType = easeType;
	return result;
}

UTweenActor* UTweenActor::DORotation(float DurTime, AActor* Actor, FRotator End, EaseType easeType, bool IsLoop)
{
	if(!Actor) return nullptr;
	UTweenActor* result = NewObject<UTweenActor>();
	result->Init(DurTime);
	result->StartRotValue = Actor->GetActorRotation().Quaternion();
	result->EndRotValue = End.Quaternion();
	result->SetLooping(IsLoop);
	result->Actor = Actor;
	result->TweenActorType = ETweenActorType::E_ActorRotation;
	result->CurEasyType = easeType;
	return result;
}

UTweenActor* UTweenActor::DOScale(float DurTime, AActor* Actor, FVector End, EaseType easeType, bool IsLoop)
{
	if(!Actor) return nullptr;
	UTweenActor* result = NewObject<UTweenActor>();
	result->Init(DurTime);
	result->StartScaleValue = Actor->GetActorScale();
	result->EndScaleValue = End;
	result->SetLooping(IsLoop);
	result->Actor = Actor;
	result->TweenActorType = ETweenActorType::E_ActorScale;
	result->CurEasyType = easeType;
	return result;
}

void UTweenActor::UpdateFunc(float deltaTime)
{
	if (Paused)return;//��ͣ״̬
	Super::UpdateFunc(deltaTime);
	if (!Actor.Get()) return;
	if (!isInit)
	{
		isInit = true;
		OnFirst.Broadcast(this);
	}
	switch (TweenActorType)
	{
		case E_ActorRelativeLocation:
			{
				FVector CurValue = StartLocValue +  Evaluate() * (EndLocValue - StartLocValue);
				if (Actor.Get())
				{
					Actor->SetActorRelativeLocation(CurValue);
				}
			}
			break;

		case E_ActorLocation:
			{
				FVector CurValue = StartLocValue +  Evaluate() * (EndLocValue - StartLocValue);
				if (Actor.Get())
				{
					Actor->SetActorLocation(CurValue);
				}
			}
			break;
		case E_ActorRelativeRotation:
			{
				FQuat CurValue = FMath::Lerp(StartRotValue,EndRotValue, Evaluate());
				if (Actor.Get())
				{
					Actor->SetActorRelativeRotation(CurValue);
				}
			}
			break;
		case E_ActorRotation:
			{
				FQuat CurValue = FMath::Lerp(StartRotValue,EndRotValue, Evaluate());
				if (Actor.Get())
				{
					Actor->SetActorRotation(CurValue);
				}
			}
			break;
		case E_ActorRelativeScale:
			{
				FVector CurValue = StartScaleValue +  Evaluate() * (EndScaleValue - StartScaleValue);
				if (Actor.Get())
				{
					Actor->SetActorRelativeScale3D(CurValue);
				}
			}
			break;
		case E_ActorScale:
			{
				FVector CurValue = StartScaleValue +  Evaluate() * (EndScaleValue - StartScaleValue);
				if (Actor.Get())
				{
					Actor->SetActorScale3D(CurValue);
				}
			}
			break;
					
	}
	OnUpdate.Broadcast(this);
	if (Finished)
	{
		OnComplete.Broadcast(this);
		return;
	}
}
