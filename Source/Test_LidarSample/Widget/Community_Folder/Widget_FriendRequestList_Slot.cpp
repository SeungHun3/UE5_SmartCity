// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FriendRequestList_Slot.h"
#include "Components/Button.h"

void UWidget_FriendRequestList_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Agree->OnClicked.AddUniqueDynamic(this, &UWidget_FriendRequestList_Slot::Click_AgreeButton);
	Button_Disagree->OnClicked.AddUniqueDynamic(this, &UWidget_FriendRequestList_Slot::Click_DisagreeButton);
}

//ģ�� �����ϱ�
void UWidget_FriendRequestList_Slot::Click_AgreeButton()
{
	Dispatcher_AgreeFriend.Broadcast(FriendInfo);
}

//ģ�� �����ϱ�
void UWidget_FriendRequestList_Slot::Click_DisagreeButton()
{
	Dispatcher_DisagreeFriend.Broadcast(FriendInfo);
}

