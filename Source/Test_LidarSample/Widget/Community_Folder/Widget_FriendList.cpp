// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FriendList.h"
#include "Widget_Friend_Slot.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include"..\..\Pawn_Player.h"
#include "..\..\ActorComponent_PlayfabStore.h"

//�ѱ� �ð��� ���缭 �α׾ƿ� �ð��� ������ش�,
FTimespan UWidget_FriendList::GetElapsedTime(FDateTime LogoutTime)
{
	FDateTime DateTime= LogoutTime;

	FDateTime NowTime = FDateTime::UtcNow();
	FTimespan NineHours(9, 0, 0);
	NowTime = NowTime + NineHours;//�ѱ� �ð��� ���缭 9�ð��� �����ش�

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

//ģ�� ��ûâ ����
void UWidget_FriendList::Click_OpenRequest()
{
	Dispatcher_OpenRequest.Broadcast();
}

//���� ���â ����
void UWidget_FriendList::Click_OpenBlock()
{
	Dispatcher_OpenBlock.Broadcast();
}

//ģ�� ����Ʈ�� �����Ѵ�.
void UWidget_FriendList::UpdateFriendList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->getFriendListEvent();
	}
}

//ģ�� ����Ʈ ������Ʈ
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

//ģ�� ��� ������Ʈ �ݹ� : ��ũ�ѹڽ��� ģ�������� �߰����ش�.
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

//ģ�� �ο��� ���ϱ�
void UWidget_FriendList::SetTotalFriendNum()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Dispatcher_CompleteFriendlist.Broadcast(Player->BP_ActorComponent_Playfab->FriendList.Num());
	}
}

//ģ��â ��ȯ�� ���� �����ִ� ���ε��� ��������Ʈ�� �������ش�.
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

//ģ�� ����
void UWidget_FriendList::DeleteFirend(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->ScriptCustom("RemoveFriendList", "targetPlayer", PlayfabInfo.PlayFabID);
	}
}

//ģ������ ���� ������ �ٲٱ�
void UWidget_FriendList::ChangeFriendToBlock(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->ScriptCustom("FriendToBlock", "targetPlayer", PlayfabInfo.TitleID);
	}
}
