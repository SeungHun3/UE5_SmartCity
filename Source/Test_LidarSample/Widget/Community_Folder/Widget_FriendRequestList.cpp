// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FriendRequestList.h"
#include "Widget_FriendRequestList_Slot.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include"..\..\Pawn_Player.h"
#include "..\..\ActorComponent_PlayfabStore.h"

void UWidget_FriendRequestList::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddUniqueDynamic(this, &UWidget_FriendRequestList::Click_OffRequest);
}

//
void UWidget_FriendRequestList::Click_OffRequest()
{
	Dispatcher_OffRequest.Broadcast();
}

//친구 요청 목록을 새로 갱신한다.
void UWidget_FriendRequestList::UpdateRequestList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		ScrollBox_FriendList->ClearChildren();
		for (auto it : Player->BP_ActorComponent_Playfab->FriendRequestList)
		{
			UWidget_FriendRequestList_Slot* SlotWidget = CreateWidget<UWidget_FriendRequestList_Slot>(GetWorld(), SpawnSlot);
			SlotWidget->SetTextTitleID(FText::FromString(it.Value.TitleID));
			SlotWidget->FriendInfo = it.Value;
			SlotWidget->Dispatcher_AgreeFriend.AddDynamic(this, &UWidget_FriendRequestList::AgreeFriend);
			SlotWidget->Dispatcher_DisagreeFriend.AddDynamic(this, &UWidget_FriendRequestList::DeleteFriend);
			ScrollBox_FriendList->AddChild(SlotWidget);
		}
	}
		
}

//친구 요청 수락하기
void UWidget_FriendRequestList::AgreeFriend(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		Player->BP_ActorComponent_Playfab->ScriptCustom("AddFriendlist", "targetPlayer", PlayfabInfo.PlayFabID);
		//수락했으니 요청 목록에서 제거하기
		DeleteFriend(PlayfabInfo);
	}
}

//친구 요청 거절하기
void UWidget_FriendRequestList::DeleteFriend(const FFriendStruct& TitleID)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		Player->BP_ActorComponent_Playfab->FriendRequestList.Remove(TitleID.TitleID);
		UpdateRequestList();
	}
}
