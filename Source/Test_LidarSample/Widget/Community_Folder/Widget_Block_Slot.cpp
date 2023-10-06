// Fill out your copyright notice in the Description page of Project Settings.


#include"Widget_Block_Slot.h"
#include "Components/Button.h"

void UWidget_Block_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Unlocked->OnClicked.AddUniqueDynamic(this, &UWidget_Block_Slot::Click_Unlock);
}

//치딘 해제 클릭
void UWidget_Block_Slot::Click_Unlock()
{
	Dispatcher_RemoveBlock.Broadcast(FriendInfo);
}
