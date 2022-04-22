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

	FVector2D Pivot(0.5f, 0.5f);
	m_SpeechBubbleComp = CreateDefaultSubobject<UWidgetSpeechBubbleComp>(TEXT("SpeechBubbleComp"));
	m_SpeechBubbleComp->SetupAttachment(RootComponent);
	m_SpeechBubbleComp->SetVisibility(false);
	m_SpeechBubbleComp->SetDrawAtDesiredSize(true);
	m_SpeechBubbleComp->SetPivot(Pivot);
	m_SpeechBubbleComp->SetWidgetSpace(EWidgetSpace::Screen);
	m_SpeechBubbleComp->SetRelativeLocation(FVector(0, 0, -100));
	m_SpeechBubbleComp->SetCanEverAffectNavigation(false);
}//WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/WB_SpeechBubble.WB_SpeechBubble'
//가까이 갈때마다 ,? 그냥 포커싱? 조작,창고안에서 훔치기
void AItemActor::Init(FName itemID, int countOrLevel)
{
	m_ItemID = itemID;

	m_nCountLevel = countOrLevel;
	
	const FItemDataRow& ItemData = UMyLib::GetItemData(m_ItemID);

	UTexture2D* t = UMyAssetManager::Get()->LoadTexture(ItemData.m_Icon);

	m_BillboardIcon->SetSprite(t);
	
	m_SpeechBubbleComp->SetVisibility(true);

	//FString Str = FString::Printf(TEXT("%s"),);

	m_SpeechBubbleComp->SetText(ItemData.m_ShowingName);
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
