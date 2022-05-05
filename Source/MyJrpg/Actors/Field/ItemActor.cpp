#include "ItemActor.h"
#include "Components/BillboardComponent.h"
#include "MyJrpg/MyLib.h"
#include "MyJrpg/Managers/MyAssetManager.h"
#include "MyJrpg/Managers/MyGameInstance.h"
#include "MyJrpg/Widgets/WidgetComponents/WidgetSpeechBubble.h"
#include "MyJrpg/Widgets/WidgetComponents/WidgetSpeechBubbleComp.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	m_BillboardIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("m_BillboardIcon"));
	m_BillboardIcon->SetHiddenInGame(false);
	m_BillboardIcon->SetupAttachment(RootComponent);//-70
	m_BillboardIcon->SetRelativeScale3D(FVector(2));
}

void AItemActor::Init(FName itemID, int countOrLevel)
{
	m_ItemSpec.m_ID = itemID;

	m_ItemSpec.m_nLvStack = countOrLevel;
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemSpec.m_ID);

	UTexture2D* t = UMyAssetManager::Get()->LoadTexture(ItemData.m_Icon);

	m_BillboardIcon->SetSprite(t);
}

void AItemActor::Obtain()//주변에서 누가 보고있으면
{
	if(HasOwnerTeamID())
	{
		switch (UMyGameInstance::Get->m_TeamKarma->GetUnitKarma(m_OwnerID))
		{
		case EKarma::Friendly:
			break;
		case EKarma::Neutral:
		case EKarma::Hate:
			UMyGameInstance::Get->m_TeamKarma->DecreaseKarma(m_OwnerID, 30);
			break;
		}
	}

	if(UMyGameInstance::Get->m_Inven->AddItem(m_ItemSpec))
	{
		UMyGameInstance::Get->m_SpawnManager->RemoveFocusActor(this);
		Destroy();
	}
}

bool AItemActor::HasOwnerTeamID() const
{
	return !m_OwnerID.IsNone();
}

float AItemActor::GetBoundHalfHeight()
{
	return 88.f;
}
