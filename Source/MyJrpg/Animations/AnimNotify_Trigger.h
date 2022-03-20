// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Trigger.generated.h"

/**
 * 
 */
UCLASS()
class MYJRPG_API UAnimNotify_Trigger : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	FName m_ID;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
