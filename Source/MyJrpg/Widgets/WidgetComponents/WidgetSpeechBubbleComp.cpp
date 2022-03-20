#include "WidgetSpeechBubbleComp.h"
#include "WidgetSpeechBubble.h"
#include "UObject/ConstructorHelpers.h"

//WidgetBlueprint'/Game/Blueprints/Widgets/World/Zone/WB_SpeechBubble.WB_SpeechBubble'

UWidgetSpeechBubbleComp::UWidgetSpeechBubbleComp()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> Found(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/World/Component/WB_SpeechBubble.WB_SpeechBubble_C'"));
	
	SetWidgetClass(Found.Class);
	
	SetManuallyRedraw(true);
}

void UWidgetSpeechBubbleComp::Hide()
{
	SetVisibility(false);
}

void UWidgetSpeechBubbleComp::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* WidgetObj = GetUserWidgetObject();

	if (!WidgetObj)
	{
		return;
	}

	UWidgetSpeechBubble* WidgetMy = Cast<UWidgetSpeechBubble>(WidgetObj);

	WidgetMy->m_OnComplete.BindUObject(this,&UWidgetSpeechBubbleComp::Hide);
}

void UWidgetSpeechBubbleComp::Speech(const FText& text)
{
	SetVisibility(true);
	
	UUserWidget* WidgetObj = GetUserWidgetObject();

	if (!WidgetObj)
	{
		return;
	}

	UWidgetSpeechBubble* WidgetMy = Cast<UWidgetSpeechBubble>(WidgetObj);

	WidgetMy->Speech(text);

	RequestRedraw();
}
