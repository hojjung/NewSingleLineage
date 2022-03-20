#include "DialogueUserWidget.h"
#include "UObject/UnrealType.h"
#include "UObject/Class.h"
#include "Dialogue.h"
#include "TimerManager.h"
#include "Kismet/KismetStringLibrary.h"

void UDialogueUserWidget::StartDialogue(AActor* speaker, UDialogue* inDial)
{
	NPCActor = 	speaker;

	InDialogue = inDial;
}

void UDialogueUserWidget::RunEventsForNode(const FDialogueNode& Node)
{
	for (UDialogueEvents* Event : Node.m_AryEvents)
	{
		if (IsValid(Event))
		{
			Event->RecieveEventTriggered(GetOwningPlayer(), NPCActor);
		}
	}
}

bool UDialogueUserWidget::CheckNodeConditionEnable(const FDialogueNode& Node)
{
	for (UDialogueConditions* Condition : Node.m_AryConditions)
	{
		if (IsValid(Condition))
		{
			if (!Condition->IsConditionMet(GetOwningPlayer(), NPCActor))
			{
				return false;
			}
		}
	}
	return true;
}

FString UDialogueUserWidget::ReplaceVarStrings(const FText& InText)
{
	FText LocalText = InText;

	TArray<FString> AryStrings = FindVarStrings(LocalText);

	for(const FString& StringW : AryStrings)
	{
		FString OutStr;
		
		if(RunStringReplacer(StringW,OutStr))
		{
			FString FormantStrW = FString::Printf(TEXT("%%%s%%"),*StringW);

			LocalText = FText::FromString(UKismetStringLibrary::Replace(LocalText.ToString(), FormantStrW, OutStr,ESearchCase::CaseSensitive));
		}
	}

	return LocalText.ToString();
}

/* If you supply this function with "charname", it'll run the function called Get_charname
 * It'll also make sure that your Get_charname function has no parameters and only returns a string
 * The resulting string will be returned in &resultString
*/
bool UDialogueUserWidget::RunStringReplacer(FString originalString, FString& resultString)
{
	const FString methodToCall = FString::Printf(TEXT("Get_%s"), *originalString);
	UFunction* Func = GetClass()->FindFunctionByName(FName(*methodToCall), EIncludeSuperFlag::IncludeSuper);
//GetClass()
	if (Func == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Function \"%s\" wasn't found on the dialogue widget."),
		       *methodToCall);
		return false;
	}

	int foundReturnStrings = 0;
	for (TFieldIterator<FProperty> It(Func); It; ++It)
	{
		FProperty* Prop = *It;

		// print property flags
		//uint64 flags = (uint64)Prop->GetPropertyFlags();
		//UE_LOG(LogTemp, Log, TEXT("Property Flags: %llu"), flags); // convert to hex: https://www.rapidtables.com/convert/number/decimal-to-hex.html

		// if it's a return type (in blueprints it's an out parameter), check that it's a string
		if (Prop->HasAllPropertyFlags(CPF_Parm | CPF_OutParm))
		{
			if (!Prop->GetCPPType().Equals(TEXT("FString")))
			{
				// if we land here, it means our method returns something other than a string
				UE_LOG(LogTemp, Error,
				       TEXT("Dialogue Plugin: Your method \"%s\" is returning something other than a string!"),
				       *methodToCall);
				return false;
			}
			else
			{
				foundReturnStrings++;
			}
		}
			// if it's a normal parameter, return false
		else if (Prop->HasAnyPropertyFlags(CPF_Parm) && !Prop->HasAnyPropertyFlags(CPF_OutParm))
		{
			// we have some parameters, but we shouldn't have them
			UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Your method \"%s\" must have no parameters!"), *methodToCall);
			return false;
		}
	}
	if (foundReturnStrings > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Dialogue Plugin: Your method \"%s\" must return only one string!"), *methodToCall);
		return false;
	}
	else if (foundReturnStrings == 0)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("Dialogue Plugin: Your method \"%s\" doesn't return anything, but must return a string!"),
		       *methodToCall);
		return false;
	}

	FString retvalue;
	ProcessEvent(Func, &retvalue);
	resultString = retvalue;

	return true;
}

/* In the supplied inText, finds all strings of that fit the pattern %string% (a single word between two percentage signs) 
 * and returns an array of them without the percentage sign.
*/
TArray<FString> UDialogueUserWidget::FindVarStrings(FText inText)
{
	TArray<FString> varStrings;

	FString totalText = inText.ToString();

	int firstPercent = -1;
	for (int i = 0; i < totalText.Len(); i++)
	{
		if (totalText[i] == '%')
		{
			if (firstPercent == -1) // if we encounter the first % sign
			{
				firstPercent = i;
			}
			else if (firstPercent + 1 == i) //if we encounter "%%", disregard the first one
			{
				firstPercent = i;
			}
			else // if we encounter second % sign
			{
				FString foundVarString = totalText.Mid(firstPercent + 1, i - firstPercent - 1);
				varStrings.AddUnique(foundVarString);
				firstPercent = -1;
			}
		}
		if (totalText[i] == ' ')
		{
			firstPercent = -1;
		}
	}

	return varStrings;
}

void UDialogueUserWidget::RunStringTypingTimer(FString originalString, float charDelay, UTextLayoutWidget* targetTextBlock)
{
	if (originalString.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Empty String"));
		return;
	}

	if (GetWorld()->GetTimerManager().GetTimerRemaining(m_TypeTimerHandle) != -1)
	{
		CompleteTyping();
	}

	SetText(FText());
	m_BlendingString.Reset();
	m_nTypeRepeatCounter = 0;
	SetTextLayout(targetTextBlock);
	m_TypingWantString = originalString;
	m_nStringLength = m_TypingWantString.Len();

	GetWorld()->GetTimerManager().SetTimer(m_TypeTimerHandle, this, &UDialogueUserWidget::RunStringTyping, charDelay,
	                                       originalString.Len() > 0);
}

void UDialogueUserWidget::RunStringTyping()
{
	m_BlendingString += m_TypingWantString[m_nTypeRepeatCounter];

	m_nTypeRepeatCounter++;

	if (m_nTypeRepeatCounter >= m_nStringLength)
	{
		CompleteTyping();
	}
	else
	{
		SetText(FText::FromString(m_BlendingString));
	}
}


void UDialogueUserWidget::CompleteTyping()
{
	GetWorld()->GetTimerManager().ClearTimer(m_TypeTimerHandle);

	SetText(FText::FromString(m_TypingWantString));
}

void UDialogueUserWidget::SetTextLayout(UTextLayoutWidget* tt)
{
	UTextBlock* tB = Cast<UTextBlock>(tt);
	
	if(tB)
	{
		m_TextBlock = tB;
	}
	else
	{
		m_RichTextBlock= Cast<URichTextBlock>(tt);
	}
}

void UDialogueUserWidget::SetText(FText tt)
{
	if(m_TextBlock)
	{
		m_TextBlock->SetText(tt);
	}
	else if(m_RichTextBlock)
	{
		m_RichTextBlock->SetText(tt);
	}
}

bool UDialogueUserWidget::IsTypingCompleteEnable()
{
	float PercentOne = (float)m_nTypeRepeatCounter/(float)m_nStringLength;

	return PercentOne>0.8f;
}



