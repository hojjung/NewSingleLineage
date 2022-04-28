// All right Reserve 2021 HereticByte

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttachEquipmentBase.generated.h"

UCLASS()
class MYJRPG_API AAttachEquipmentBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttachEquipmentBase();
	
protected:
	UPROPERTY(EditAnywhere)
	FName m_SocketID;
	
public:	
	FName GetSocketName();

	void SetActive(bool b);
};

