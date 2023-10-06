// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Friend_Slot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

//로그아웃 시간을 입력해준다.
void UWidget_Friend_Slot::SetLogoutTimeToText(FTimespan InputTimeSpan)
{
	int mtime=0;
	FString strr ="";

	if (InputTimeSpan.GetTotalDays() >= 1)
	{
		mtime = (int)InputTimeSpan.GetTotalDays();
		strr = FString::Printf(TEXT("Login %d days ago"), mtime);
	}
	else if (InputTimeSpan.GetTotalHours() >= 1)
	{
		mtime = (int)InputTimeSpan.GetTotalHours();
		strr = FString::Printf(TEXT("Login %d hours ago"), mtime);
	}
	else if (InputTimeSpan.GetTotalMinutes() >= 1)
	{
		mtime = (int)InputTimeSpan.GetTotalMinutes();
		strr = FString::Printf(TEXT("Login %d minutes ago"), mtime);
	}
	else if (InputTimeSpan.GetTotalSeconds() >= 1)
	{
		mtime = (int)InputTimeSpan.GetTotalSeconds();
		strr = FString::Printf(TEXT("Login %d seconds ago"), mtime);
	}

	TextBlock_Logout->SetText(FText::FromString(strr));
}

void UWidget_Friend_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Delete->OnClicked.AddUniqueDynamic(this, &UWidget_Friend_Slot::Click_Delete);
	Button_Block->OnClicked.AddUniqueDynamic(this, &UWidget_Friend_Slot::Click_Block);
}

//친구 차단하기
void UWidget_Friend_Slot::Click_Block()
{
	Dispatcher_BlockFriend.Broadcast(FriendInfo);
}

//친구 삭제하기
void UWidget_Friend_Slot::Click_Delete()
{
	Dispatcher_DeleteFriend.Broadcast(FriendInfo);
}
