// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Option.h"
#include "Components/Overlay.h"
#include "Components/Button.h"

void UWidget_Option::NativeOnInitialized() 
{
	Super::NativeOnInitialized();

	Tab_BtnSound->OnClicked.AddDynamic(this, &UWidget_Option::Clicked_Sound);
	Tab_BtnAlarm->OnClicked.AddDynamic(this, &UWidget_Option::Clicked_Alarm);
	Tab_BtnAccount->OnClicked.AddDynamic(this, &UWidget_Option::Clicked_Account);
}
//옵션 탭 선택
void UWidget_Option::SetOptionStyle(enum_OptionStyle style) 
{
	switch (style)
	{
	case enum_OptionStyle::OptionSound:
		Overlay_Sound->SetVisibility(ESlateVisibility::Visible);
		Overlay_Alarm->SetVisibility(ESlateVisibility::Collapsed);
		Overlay_Account->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case enum_OptionStyle::OptionAlarm:
		Overlay_Sound->SetVisibility(ESlateVisibility::Collapsed);
		Overlay_Alarm->SetVisibility(ESlateVisibility::Visible);
		Overlay_Account->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case enum_OptionStyle::OptionAccount:
		Overlay_Sound->SetVisibility(ESlateVisibility::Collapsed);
		Overlay_Alarm->SetVisibility(ESlateVisibility::Collapsed);
		Overlay_Account->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}
//선택 탭 버튼 초기화
void UWidget_Option::InitTabButton()
{
	Clicked_Sound();
}
//사운드 버튼 클릭
void UWidget_Option::Clicked_Sound()
{
	CurrentTabNumber = 0;
	SetOptionStyle(enum_OptionStyle::OptionSound);
	UpdateButtonStyle();
}
// 알림 버튼 클릭
void UWidget_Option::Clicked_Alarm()
{
	CurrentTabNumber = 1;
	SetOptionStyle(enum_OptionStyle::OptionAlarm);
	UpdateButtonStyle();
}
//계정 버튼 클릭
void UWidget_Option::Clicked_Account()
{
	CurrentTabNumber = 2;
	SetOptionStyle(enum_OptionStyle::OptionAccount);
	UpdateButtonStyle();
}
