// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStackCalculator.h"

#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "MyJrpg/MyJrpg.h"

void UWidgetStackCalculator::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_TextNumber->OnTextChanged.AddDynamic(this,&UWidgetStackCalculator::OnTextChanged);

	m_BtnMinus->IsFocusable = false;
	m_BtnPlus->IsFocusable = false;
	m_Btn00->IsFocusable = false;
	m_Btn01->IsFocusable = false;
	m_Btn02->IsFocusable = false;
	m_Btn03->IsFocusable = false;
	m_Btn04->IsFocusable = false;
	m_Btn05->IsFocusable = false;
	m_Btn06->IsFocusable = false;
	m_Btn07->IsFocusable = false;
	m_Btn08->IsFocusable = false;
	m_Btn09->IsFocusable = false;
	m_BtnErase->IsFocusable = false;
	m_BtnAc->IsFocusable = false;
	m_BtnAccept->IsFocusable = false;
	m_BtnCancel->IsFocusable = false;
	m_BtnPlus001->IsFocusable = false;
	m_BtnPlus010->IsFocusable = false;
	m_BtnPlus100->IsFocusable = false;
	m_BtnMax->IsFocusable = false;
	//
	m_BtnMinus->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnMinus);
	m_BtnPlus->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnPlus);
	m_Btn00->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On00);
	m_Btn01->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On01);
	m_Btn02->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On02);
	m_Btn03->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On03);
	m_Btn04->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On04);
	m_Btn05->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On05);
	m_Btn06->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On06);
	m_Btn07->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On07);
	m_Btn08->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On08);
	m_Btn09->OnClicked.AddDynamic(this,&UWidgetStackCalculator::On09);
	m_BtnErase->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnErase);
	m_BtnAc->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnClearToZero);
	m_BtnAccept->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnAccpet);
	m_BtnCancel->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnClose);
	m_BtnPlus001->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnPlus);
	m_BtnPlus010->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnPlus010);
	m_BtnPlus100->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnPlus100);
	m_BtnMax->OnClicked.AddDynamic(this,&UWidgetStackCalculator::OnMax);
	//
	OnClearToZero();
}

void UWidgetStackCalculator::OnTextChanged(const FText& text)
{
	FString NewText = text.ToString();

	if(NewText.IsNumeric())
	{
		m_LastEnteredNumber = text;
	}
	else
	{
		m_TextNumber->SetText(m_LastEnteredNumber);
	}

	PRINTF("UWidgetStackCalculator::Number:%d",m_nNumber);
}

void UWidgetStackCalculator::SetAppendNumber(int number)
{
	if(m_nNumber<=0)//NoAppend
	{
		m_nNumber = number;
		
		SetTextNumber(m_nNumber);
	}
	else
	{
		int MaxAmount = GetMaxAmount();
		
		FString Number = UKismetStringLibrary::Conv_IntToString(number);

		FString OldText = m_TextNumber->GetText().ToString();

		FString NewStr = OldText+Number;

		m_nNumber = FCString::Atoi(*NewStr);

		if(m_nNumber > MaxAmount)
		{
			m_nNumber = MaxAmount;

			NewStr = UKismetStringLibrary::Conv_IntToString(m_nNumber);
		}

		m_TextNumber->SetText(FText::FromString(NewStr));
	}
}

void UWidgetStackCalculator::SetTextNumber(int numberNew)
{
	m_nNumber = numberNew;

	if (m_nNumber > 0)
	{
		int MaxAmount = GetMaxAmount(); //업데이트가 외부에서 되야함

		if (m_nNumber > MaxAmount)
		{
			m_nNumber = MaxAmount;
		}
	}
	
	FString Number = UKismetStringLibrary::Conv_IntToString(m_nNumber);;

	m_TextNumber->SetText(FText::FromString(Number));
}

void UWidgetStackCalculator::OnErase()
{
	FString OldText = m_TextNumber->GetText().ToString();

	int Number = FCString::Atoi(*OldText);

	if(Number<10)
	{
		Number = 0;
	}
	else
	{
		Number /= 10;	
	}
	m_nNumber = Number;

	m_TextNumber->SetText(FText::FromString(UKismetStringLibrary::Conv_IntToString(m_nNumber)));
}

void UWidgetStackCalculator::On00()
{
	SetAppendNumber(0);
}

void UWidgetStackCalculator::On01()
{
	SetAppendNumber(1);
}

void UWidgetStackCalculator::On02()
{
	SetAppendNumber(2);
}

void UWidgetStackCalculator::On03()
{
	SetAppendNumber(3);
}

void UWidgetStackCalculator::On04()
{
	SetAppendNumber(4);
}

void UWidgetStackCalculator::On05()
{
	SetAppendNumber(5);
}

void UWidgetStackCalculator::On06()
{
	SetAppendNumber(6);
}

void UWidgetStackCalculator::On07()
{
	SetAppendNumber(7);
}

void UWidgetStackCalculator::On08()
{
	SetAppendNumber(8);
}

void UWidgetStackCalculator::On09()
{
	SetAppendNumber(9);
}

void UWidgetStackCalculator::OnMinus()
{
	m_nNumber--;

	if(m_nNumber<0)
	{
		m_nNumber = 0;
	}

	SetTextNumber(m_nNumber);
}

void UWidgetStackCalculator::OnPlus()
{
	m_nNumber++;

	int Max = GetMaxAmount();

	if(m_nNumber>Max)
	{
		m_nNumber = Max;
	}

	SetTextNumber(m_nNumber);
}

int UWidgetStackCalculator::GetMaxAmount()
{
	if(!m_OnGetMax.IsBound())
	{
		return FGlobalVariable::INVEN_MAXSTACK;	
	}
	
	return m_OnGetMax.Execute();
}

void UWidgetStackCalculator::OnClearToZero()
{
	SetTextNumber(0);
}

void UWidgetStackCalculator::OnClose()
{
	OnClearToZero();

	m_OnNumberAccept.Clear();

	m_OnGetMax.Unbind();
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UWidgetStackCalculator::OnAccpet()
{
	m_OnNumberAccept.Broadcast(m_nNumber);
	
	OnClose();
}

void UWidgetStackCalculator::OnPlus010()
{
	m_nNumber+=10;

	int Max = GetMaxAmount();

	if(m_nNumber>Max)
	{
		m_nNumber = Max;
	}

	SetTextNumber(m_nNumber);
}

void UWidgetStackCalculator::OnPlus100()
{
	m_nNumber+=100;

	int Max = GetMaxAmount();

	if(m_nNumber>Max)
	{
		m_nNumber = Max;
	}

	SetTextNumber(m_nNumber);
}

void UWidgetStackCalculator::OnMax()
{
	m_nNumber=GetMaxAmount();

	SetTextNumber(m_nNumber);
}

void UWidgetStackCalculator::Open(int initAmount)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_nNumber = initAmount;
	SetTextNumber(m_nNumber);
}
