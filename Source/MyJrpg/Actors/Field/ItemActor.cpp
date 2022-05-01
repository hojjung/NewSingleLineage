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
}

void AItemActor::Init(FName itemID, int countOrLevel)
{
	m_ItemID = itemID;

	m_nCountLevel = countOrLevel;
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemID);

	UTexture2D* t = UMyAssetManager::Get()->LoadTexture(ItemData.m_Icon);

	m_BillboardIcon->SetSprite(t);
}

void AItemActor::Obtain()//주변에서 누가 보고있으면
{
	if(HasOwnerTeamID())
	{
		//해당 팀ID를 가진 사람이 보고있으면
	}
}

bool AItemActor::HasOwnerTeamID() const
{
	return !m_OwnerID.IsNone();
}

float AItemActor::GetBoundHalfHeight()
{
	return 44;
}
