// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "NpcActor.h"
#include "QuestActor.generated.h"

UCLASS()
class MYJRPG_API AQuestActor : public ANpcActor
{
	GENERATED_BODY()
	
public:	
	AQuestActor();

protected:
	virtual void OnInteract() override;
};
