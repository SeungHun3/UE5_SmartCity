// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Community_Slot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UWidget_Community_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}

//닉네임 설정
void UWidget_Community_Slot::SetTextTitleID(const FText &TitleID)
{
	FText co_TitleID = TitleID;
	if (TextBlock_TitleID->IsValidLowLevel())
	{
		TextBlock_TitleID->SetText(co_TitleID);
	}
	
}
