// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FriendList.h"
#include "Widget_Friend_Slot.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include"..\..\Pawn_Player.h"
#include "..\..\ActorComponent_PlayfabStore.h"

//한국 시간에 맞춰서 로그아웃 시간을 계산해준다,
FTimespan UWidget_FriendList::GetElapsedTime(FDateTime LogoutTime)
{
	FDateTime DateTime= LogoutTime;

	FDateTime NowTime = FDateTime::UtcNow();
	FTimespan NineHours(9, 0, 0);
	NowTime = NowTime + NineHours;//한국 시간에 맞춰서 9시간을 더해준다

	FTimespan ElapsedTime = NowTime - DateTime;
	
	UE_LOG(LogTemp, Log, TEXT("// GetLastLogoutSeconds :: %s"), *NowTime.ToString());

	UE_LOG(LogTemp, Log, TEXT("// GetLastLogoutSeconds :: %d ,%d ,%d"), ElapsedTime.GetHours(), ElapsedTime.GetMinutes(), ElapsedTime.GetSeconds());

	return ElapsedTime;
}

void UWidget_FriendList::ShowLoginTIme()
{

}

void UWidget_FriendList::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Request->OnClicked.AddUniqueDynamic(this, &UWidget_FriendList::Click_OpenRequest);
	Button_Block->OnClicked.AddUniqueDynamic(this, &UWidget_FriendList::Click_OpenBlock);
}

//친구 요청창 열기
void UWidget_FriendList::Click_OpenRequest()
{
	Dispatcher_OpenRequest.Broadcast();
}

//차단 목록창 열기
void UWidget_FriendList::Click_OpenBlock()
{
	Dispatcher_OpenBlock.Broadcast();
}

//친구 리스트를 갱신한다.
void UWidget_FriendList::UpdateFriendList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->getFriendListEvent();
	}
}

//친구 리스트 업데이트
void UWidget_FriendList::OpenWBFriendList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->UpdateFriendSlot.AddDynamic(this, &UWidget_FriendList::CompleteFriendList);
		Player->BP_ActorComponent_Playfab->UpdateFriend.AddDynamic(this, &UWidget_FriendList::SetTotalFriendNum);
		UpdateFriendList();
	}
}

//친구 목록 업데이트 콜백 : 스크롤박스에 친구위젯을 추가해준다.
void UWidget_FriendList::CompleteFriendList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		ScrollBox_FriendList->ClearChildren();
		for (auto it : Player->BP_ActorComponent_Playfab->FriendList)
		{
			UWidget_Friend_Slot* SlotWidget = CreateWidget<UWidget_Friend_Slot>(GetWorld(), SpawnSlot);
			ScrollBox_FriendList->AddChild(SlotWidget);
			SlotWidget->SetTextTitleID(FText::FromString(it.Value.TitleID));
			SlotWidget->SetLogoutTimeToText(GetElapsedTime(it.Value.LastLogin));
			SlotWidget->FriendInfo = it.Value;
			SlotWidget->Dispatcher_DeleteFriend.AddDynamic(this, &UWidget_FriendList::DeleteFirend);
			SlotWidget->Dispatcher_BlockFriend.AddDynamic(this, &UWidget_FriendList::ChangeFriendToBlock);
		}
	}
}

//친구 인원수 구하기
void UWidget_FriendList::SetTotalFriendNum()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Dispatcher_CompleteFriendlist.Broadcast(Player->BP_ActorComponent_Playfab->FriendList.Num());
	}
}

//친구창 전환시 같이 쓰고있는 바인딩된 델리게이트를 해제해준다.
void UWidget_FriendList::CloseWBFriendList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		ScrollBox_FriendList->ClearChildren();
		Player->BP_ActorComponent_Playfab->UpdateFriendSlot.RemoveDynamic(this, &UWidget_FriendList::CompleteFriendList);
		Player->BP_ActorComponent_Playfab->UpdateFriend.RemoveDynamic(this, &UWidget_FriendList::SetTotalFriendNum);
	}
}

//친구 삭제
void UWidget_FriendList::DeleteFirend(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->ScriptCustom("RemoveFriendList", "targetPlayer", PlayfabInfo.PlayFabID);
	}
}

//친구에서 차단 유저로 바꾸기
void UWidget_FriendList::ChangeFriendToBlock(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->ScriptCustom("FriendToBlock", "targetPlayer", PlayfabInfo.TitleID);
	}
}
